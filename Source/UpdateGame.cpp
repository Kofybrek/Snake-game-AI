#include <array>
#include <chrono>
#include <random>

#include "Headers/Global.hpp"
#include "Headers/RandomGenerator.hpp"
#include "Headers/AddFood.hpp"
#include "Headers/Snake.hpp"

void update_game(std::vector<Snake>& i_snakes, gbl::Map& i_map, RandomGenerator& i_random_generator)
{
	for (Snake& snake : i_snakes)
	{
		snake.update_direction();
		snake.move();
	}

	//Here we're clearing the map.
	for (std::array<gbl::Cell, gbl::MAP::ROWS>& column : i_map)
	{
		for (gbl::Cell& cell : column)
		{
			cell.id = gbl::GAME::TOTAL_SNAKES;
			cell.total_snake_bodies = 0;
		}
	}

	for (Snake& snake : i_snakes)
	{
		snake.update_map();
	}

	for (Snake& snake : i_snakes)
	{
		snake.update_status();
	}

	for (std::array<gbl::Cell, gbl::MAP::ROWS>& column : i_map)
	{
		for (gbl::Cell& cell : column)
		{
			if (0 < cell.total_snake_bodies && 1 == cell.food)
			{
				//Here we're removing food if any snake has eaten it, and putting new food on the map.
				cell.food = 0;

				add_food(i_map, i_random_generator);
			}
		}
	}
}