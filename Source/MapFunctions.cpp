#include <array>
#include <chrono>
#include <random>

#include "Headers/Global.hpp"
#include "Headers/RandomGenerator.hpp"

gbl::Position<> get_random_cell(const gbl::Map& i_map, RandomGenerator& i_random_generator)
{
	bool found_empty_cell = 0;

	for (const std::array<gbl::Cell, gbl::MAP::ROWS>& column : i_map)
	{
		if (1 == found_empty_cell)
		{
			break;
		}

		for (const gbl::Cell& cell : column)
		{
			if (0 == cell.food && 0 == cell.total_snake_bodies)
			{
				found_empty_cell = 1;

				//I really wish 2 * break existed so I could use it here.
				break;
			}
		}
	}

	//If we don't check this, there's a chance that the loop will go on forever.
	if (1 == found_empty_cell)
	{
		unsigned char cell_x = 0;
		unsigned char cell_y = 0;

		do
		{
			cell_x = i_random_generator.get_random_int<unsigned short>(gbl::MAP::COLUMNS - 1, 0);
			cell_y = i_random_generator.get_random_int<unsigned short>(gbl::MAP::ROWS - 1, 0);
		}
		while (0 < i_map[cell_x][cell_y].total_snake_bodies || 1 == i_map[cell_x][cell_y].food);

		return gbl::Position<>(cell_x, cell_y);
	}

	return gbl::Position<>(-1, -1);
}