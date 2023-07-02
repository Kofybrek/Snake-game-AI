#include <array>
#include <chrono>
#include <random>

#include "Headers/Global.hpp"
#include "Headers/RandomGenerator.hpp"
#include "Headers/MapFunctions.hpp"

void add_food(gbl::Map& i_map, RandomGenerator& i_random_generator, const unsigned short i_food_amount)
{
	for (unsigned short a = 0; a < i_food_amount; a++)
	{
		gbl::Position<> food_position = get_random_cell(i_map, i_random_generator);

		//Because get_random_cell() returns Position(-1, -1) if it can't find an emtpy cell.
		//Hey, I misspelled the word "empty". Did you notice it?
		//I won't fix it though.
		//Enjoy!
		if (-1 != food_position.x)
		{
			i_map[food_position.x][food_position.y].food = 1;
		}
		else
		{
			return;
		}
	}
}