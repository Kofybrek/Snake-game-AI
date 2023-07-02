#pragma once

class Snake
{
	bool dead;

	float fitness;

	unsigned short food_timer;
	unsigned short grow_length;
	unsigned short id;

	std::vector<gbl::Position<>> body;

	gbl::SNAKE::Direction direction;

	gbl::Map* map_pointer;

	gbl::Vector_3d weights;

	RandomGenerator* random_generator_pointer;
public:
	Snake(const unsigned short i_id, gbl::Map& i_map, RandomGenerator& i_random_generator);

	bool get_dead() const;
	bool operator>(const Snake& i_snake) const;
	bool operator<(const Snake& i_snake) const;

	float get_fitness() const;

	unsigned short get_length() const;

	void crossover(const gbl::Vector_3d& i_snake_0_weights, const gbl::Vector_3d& i_snake_1_weights);
	void generate_weights();
	void move();
	void reset();
	void update_direction();
	void update_map() const;
	void update_status();

	std::array<float, gbl::AI::INPUT_NEURONS> calculate_inputs() const;

	gbl::Vector_3d get_weights() const;
};