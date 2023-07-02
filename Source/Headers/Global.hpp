#pragma once

namespace gbl
{
	namespace AI
	{
		constexpr unsigned char INPUT_NEURONS = 8;
		//This is in percent.
		constexpr unsigned char MUTATION_RATE = 2;
		constexpr unsigned char OUTPUT_NEURONS = 3;

		//The size of this array represents the number of hidden layers.
		//And the numbers in the array represent the number of neurons in each layer.
		constexpr std::array<unsigned char, 1> HIDDEN_NEURONS = {8};
		//The numbers in this array represent the number of bias neurons in each layer not including the output layer.
		constexpr std::array<unsigned char, 1 + HIDDEN_NEURONS.size()> BIAS_NEURONS = {1, 1};
	}

	namespace GAME
	{
		//How many frames should pass before we update the game.
		constexpr unsigned char FRAMES_PER_UPDATE = 1;

		constexpr unsigned short START_FOOD_AMOUNT = 64;
		constexpr unsigned short TOTAL_SNAKES = 64;
	}

	namespace MAP
	{
		constexpr unsigned char CELL_OUTLINE_THICKNESS = 1;
		constexpr unsigned char CELL_SIZE = 8;
		constexpr unsigned char COLUMNS = 124;
		constexpr unsigned char ROWS = 88;

		constexpr unsigned short ORIGIN_X = 280;
		constexpr unsigned short ORIGIN_Y = 8;
	}

	namespace SCREEN
	{
		constexpr unsigned char RESIZE = 1;

		constexpr unsigned short HEIGHT = 720;
		constexpr unsigned short WIDTH = 1280;

		constexpr std::chrono::microseconds FRAME_DURATION(16667);
	}

	namespace SNAKE
	{
		//We'll use this to kill snakes when they don't eat for a long time.
		//Cruel, I know.
		constexpr unsigned short FOOD_TIMER_DURATION = 1920;

		enum Direction
		{
			Right,
			Up,
			Left,
			Down,
		};
	}

	struct Cell
	{
		bool food;

		//We'll use this to color the snakes.
		unsigned short id;
		unsigned short total_snake_bodies;

		Cell(const bool i_food = 0, const unsigned short i_id = GAME::TOTAL_SNAKES, const unsigned short i_total_snake_bodies = 0) :
			food(i_food),
			id(i_id),
			total_snake_bodies(i_total_snake_bodies)
		{

		}
	};

	template <typename value_type = short>
	struct Position
	{
		value_type x;
		value_type y;

		Position(const value_type i_x = 0, const value_type i_y = 0) :
			x(i_x),
			y(i_y)
		{

		}

		//Not sure if I did this correctly. Let's hope I did.
		bool operator==(const Position& i_position)
		{
			return x == i_position.x && y == i_position.y;
		}

		bool operator!=(const Position& i_position)
		{
			return x != i_position.x || y != i_position.y;
		}
	};

	using Map = std::array<std::array<Cell, MAP::ROWS>, MAP::COLUMNS>;

	//I heard you like vectors...
	using Vector_1d = std::vector<float>;
	using Vector_2d = std::vector<std::vector<float>>;
	using Vector_3d = std::vector<std::vector<std::vector<float>>>;
}