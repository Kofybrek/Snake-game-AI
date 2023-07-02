#pragma once

template <typename value_type = short>
bool inside_map(const gbl::Position<value_type>& i_position)
{
	return 0 <= i_position.x && 0 <= i_position.y && gbl::MAP::COLUMNS > i_position.x && gbl::MAP::ROWS > i_position.y;
}

gbl::Position<> get_random_cell(const gbl::Map& i_map, RandomGenerator& i_random_generator);