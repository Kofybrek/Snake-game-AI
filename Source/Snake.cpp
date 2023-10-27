#include <array>
#include <chrono>
#include <random>
#include <algorithm>

#include "Headers/Global.hpp"
#include "Headers/ForwardPropagation.hpp"
#include "Headers/RandomGenerator.hpp"
#include "Headers/MapFunctions.hpp"
#include "Headers/Snake.hpp"

Snake::Snake(const unsigned short i_id, gbl::Map& i_map, RandomGenerator& i_random_generator) :
	id(i_id),
	map_pointer(&i_map),
	random_generator_pointer(&i_random_generator)
{
	generate_weights();
	reset();
}

bool Snake::get_dead() const
{
	return dead;
}

bool Snake::operator>(const Snake& i_snake) const
{
	return fitness > i_snake.get_fitness();
}

bool Snake::operator<(const Snake& i_snake) const
{
	return fitness < i_snake.get_fitness();
}

float Snake::get_fitness() const
{
	return fitness;
}

unsigned short Snake::get_length() const
{
	return body.size();
}

void Snake::crossover(const gbl::Vector_3d& i_snake_0_weights, const gbl::Vector_3d& i_snake_1_weights)
{
	for (unsigned char a = 0; a < weights.size(); a++)
	{
		for (unsigned char b = 0; b < weights[a].size(); b++)
		{
			for (unsigned char c = 0; c < weights[a][b].size(); c++)
			{
				//We could use rand() here though since it's only 0 and 1.
				if (0 == random_generator_pointer->get_random_int<unsigned short>(1, 0))
				{
					weights[a][b][c] = i_snake_0_weights[a][b][c];
				}
				else
				{
					weights[a][b][c] = i_snake_1_weights[a][b][c];
				}

				if (gbl::AI::MUTATION_RATE > random_generator_pointer->get_random_int<unsigned short>(99, 0))
				{
					weights[a][b][c] = random_generator_pointer->get_random_real<float>(1, -1);
				}
			}
		}
	}
}

void Snake::generate_weights()
{
	weights.resize(1 + gbl::AI::HIDDEN_NEURONS.size());

	//This took me so long to set up.
	for (unsigned char a = 0; a < weights.size(); a++)
	{
		unsigned char size;

		if (a < weights.size() - 1)
		{
			size = gbl::AI::HIDDEN_NEURONS[a];
		}
		else
		{
			size = gbl::AI::OUTPUT_NEURONS;
		}

		if (0 == a)
		{
			weights[0].resize(size, gbl::Vector_1d(gbl::AI::INPUT_NEURONS + gbl::AI::BIAS_NEURONS[0]));
		}
		else
		{
			weights[a].resize(size, gbl::Vector_1d(gbl::AI::HIDDEN_NEURONS[a - 1] + gbl::AI::BIAS_NEURONS[a]));
		}

		for (gbl::Vector_1d& neuron : weights[a])
		{
			for (float& weight : neuron)
			{
				weight = random_generator_pointer->get_random_real<float>(1, -1);
			}
		}
	}
}

void Snake::move()
{
	if (0 == dead)
	{
		//You can also make grow_length support negative numbers and use that to add shrinking for the snakes.
		if (0 < grow_length)
		{
			grow_length--;

			body.push_back(body.back());
		}

		for (unsigned short a = body.size() - 1; 0 < a; a--)
		{
			body[a].x = body[a - 1].x;
			body[a].y = body[a - 1].y;
		}

		switch (direction)
		{
			case gbl::SNAKE::Direction::Down:
			{
				body[0].y++;

				break;
			}
			case gbl::SNAKE::Direction::Left:
			{
				body[0].x--;

				break;
			}
			case gbl::SNAKE::Direction::Right:
			{
				body[0].x++;

				break;
			}
			case gbl::SNAKE::Direction::Up:
			{
				body[0].y--;
			}
		}
	}
}

void Snake::reset()
{
	dead = 0;

	fitness = 0;

	//-1 because... because... damn, I don't remember why we're subtracting 1.
	//But I'm pretty sure it was important so you better not touch it.
	food_timer = gbl::SNAKE::FOOD_TIMER_DURATION - 1;
	grow_length = 0;

	body.clear();
	body.push_back(get_random_cell(*map_pointer, *random_generator_pointer));

	direction = gbl::SNAKE::Direction::Right;
}

void Snake::update_direction()
{
	if (0 == dead)
	{
		unsigned char ai_decision;

		gbl::Vector_1d outputs = forward_propagation(calculate_inputs(), weights);

		//We have 3 outputs.
		//1 - Turn left
		//2 - Don't turn
		//3 - Turn right
		//We pick the decision with the highest output value.
		ai_decision = std::max_element(outputs.begin(), outputs.end()) - outputs.begin();
		ai_decision = (3 + direction + ai_decision) % 4;

		switch (ai_decision)
		{
			case 0:
			{
				if (gbl::SNAKE::Direction::Left != direction)
				{
					direction = gbl::SNAKE::Direction::Right;
				}

				break;
			}
			case 1:
			{
				if (gbl::SNAKE::Direction::Down != direction)
				{
					direction = gbl::SNAKE::Direction::Up;
				}

				break;
			}
			case 2:
			{
				if (gbl::SNAKE::Direction::Right != direction)
				{
					direction = gbl::SNAKE::Direction::Left;
				}

				break;
			}
			case 3:
			{
				if (gbl::SNAKE::Direction::Up != direction)
				{
					direction = gbl::SNAKE::Direction::Down;
				}
			}
		}
	}
}

void Snake::update_map() const
{
	for (const gbl::Position<>& body_part : body)
	{
		if (1 == inside_map(body_part))
		{
			if (0 == dead)
			{
				//If there are more than 1 snake body in 1 cell, we're gonna draw the snake with the lowest id.
				//There's no logical reason why we're doing this.
				//I'm just a crazy idiot.
				map_pointer->at(body_part.x).at(body_part.y).id = std::min(id, map_pointer->at(body_part.x).at(body_part.y).id);
				map_pointer->at(body_part.x).at(body_part.y).total_snake_bodies++;
			}
			else
			{
				//Uncomment this if you want snakes to eat each other.
				//map_pointer->at(body_part.x).at(body_part.y).food = 1;
			}
		}
	}
}

void Snake::update_status()
{
	if (0 == dead)
	{
		if (0 == inside_map(body[0]))
		{
			dead = 1;
		}
		else if (1 < map_pointer->at(body[0].x).at(body[0].y).total_snake_bodies)
		{
			//This means that snake's head touched its own body or some other snake's body.
			dead = 1;
		}
		else if (1 == map_pointer->at(body[0].x).at(body[0].y).food)
		{
			fitness++;

			food_timer = gbl::SNAKE::FOOD_TIMER_DURATION;
			grow_length++;
		}
		else
		{
			//I noticed that some snakes get stuck in an infinite loop. So to prevent them from moving forever, I made it so that they die when they don't eat food for a long time.
			food_timer--;

			if (0 == food_timer)
			{
				dead = 1;
			}
		}
	}
}

std::array<float, gbl::AI::INPUT_NEURONS> Snake::calculate_inputs() const
{
	//I actually wanted to write a formula for this but then I thought "Why bother when I don't have to bother?"
	std::array<char, 8> step_x = {1, 1, 0, -1, -1, -1, 0, 1};
	std::array<char, 8> step_y = {0, -1, -1, -1, 0, 1, 1, 1};

	std::array<float, gbl::AI::INPUT_NEURONS> inputs = {};

	//There are 8 inputs for 8 directions.
	//If the first thing we see in one direction is food, the input value for that direction will be calculated based on the distance from the head of the snake to the cell with food. If the distance is high, the input value will be low, and vice versa.
	//We do the same thing when the first thing we see is the thing that the snake should avoid like the body of the snake or the border of the map. The only difference is that we multiply the result by -1.

	for (unsigned char a = 2 * direction; ; a = (1 + a) % 8)
	{
		unsigned char input_index = (a + 2 * (4 - direction)) % 8;

		unsigned short distance = 0;

		gbl::Position<> cell_position = body[0];

		while (1)
		{
			distance++;

			cell_position.x += step_x[a];
			cell_position.y += step_y[a];

			if (1 == inside_map(cell_position))
			{
				if (1 == map_pointer->at(cell_position.x).at(cell_position.y).food)
				{
					inputs[input_index] = std::max(gbl::MAP::COLUMNS, gbl::MAP::ROWS) - distance;

					break;
				}

				if (0 < map_pointer->at(cell_position.x).at(cell_position.y).total_snake_bodies)
				{
					inputs[input_index] = distance - std::max(gbl::MAP::COLUMNS, gbl::MAP::ROWS);

					break;
				}
			}
			else
			{
				inputs[input_index] = distance - std::max(gbl::MAP::COLUMNS, gbl::MAP::ROWS);

				break;
			}
		}

		if (a == (7 + 2 * direction) % 8)
		{
			break;
		}
	}

	return inputs;
}

gbl::Vector_3d Snake::get_weights() const
{
	return weights;
}