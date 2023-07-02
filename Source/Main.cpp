#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "Headers/DrawText.hpp"
#include "Headers/Global.hpp"
#include "Headers/HueToRgb.hpp"
#include "Headers/RandomGenerator.hpp"
#include "Headers/AddFood.hpp"
#include "Headers/Snake.hpp"
#include "Headers/UpdateGame.hpp"

//   _________
//  |         |
//  | [o] [o] |
//  |    _    |
//  |_________|
//    /|  |\
//   | |  | \
//   | |  | |
//   | |  | |
//
//I drew myself. I tried my best so shut up.

int main()
{
	//I used this when I was recording videos for editing.
	//You probably won't use this (unless you wanna plagiarize my videos).
	bool pause = 1;

	unsigned total_deaths = 0;
	unsigned total_length = 0;

	unsigned char update_timer = gbl::GAME::FRAMES_PER_UPDATE - 1;

	unsigned short best_score = 0;
	unsigned short current_best_score = 0;
	unsigned short updates_per_frame = 1;

	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	std::vector<Snake> snakes;

	sf::Event event;

	sf::RectangleShape cell_shape(sf::Vector2f(gbl::MAP::CELL_SIZE - gbl::MAP::CELL_OUTLINE_THICKNESS, gbl::MAP::CELL_SIZE - gbl::MAP::CELL_OUTLINE_THICKNESS));

	sf::RenderWindow window(sf::VideoMode(gbl::SCREEN::RESIZE * gbl::SCREEN::WIDTH, gbl::SCREEN::RESIZE * gbl::SCREEN::HEIGHT), "Snake game AI", sf::Style::Close);
	window.setView(sf::View(sf::FloatRect(0, 0, gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT)));

	sf::Texture font_texture;
	font_texture.loadFromFile("Resources/Images/Font.png");

	gbl::Map map;

	RandomGenerator random_generator;

	add_food(map, random_generator, gbl::GAME::START_FOOD_AMOUNT);

	for (unsigned short a = 0; a < gbl::GAME::TOTAL_SNAKES; a++)
	{
		snakes.push_back(Snake(a, map, random_generator));
		snakes.back().update_map();
	}

	previous_time = std::chrono::steady_clock::now();

	while (1 == window.isOpen())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		while (gbl::SCREEN::FRAME_DURATION <= lag)
		{
			lag -= gbl::SCREEN::FRAME_DURATION;

			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
					case sf::Event::Closed:
					{
						window.close();

						break;
					}
					case sf::Event::KeyPressed:
					{
						switch (event.key.code)
						{
							case sf::Keyboard::Down:
							{
								if (1 < updates_per_frame)
								{
									updates_per_frame /= 2;
								}

								break;
							}
							case sf::Keyboard::Space:
							{
								pause = 1 - pause;

								break;
							}
							case sf::Keyboard::Up:
							{
								updates_per_frame *= 2;
							}
						}
					}
				}
			}

			if (0 == pause)
			{
				if (0 == update_timer)
				{
					update_timer = gbl::GAME::FRAMES_PER_UPDATE - 1;

					for (unsigned short a = 0; a < updates_per_frame; a++)
					{
						total_length = 0;

						current_best_score = 0;

						std::sort(snakes.begin(), snakes.end(), std::greater());

						for (unsigned short b = 0; b < gbl::GAME::TOTAL_SNAKES; b++)
						{
							total_length += snakes[b].get_length();

							if (current_best_score < snakes[b].get_length())
							{
								current_best_score = snakes[b].get_length();
							}

							if (1 == snakes[b].get_dead())
							{
								total_deaths++;

								if (1 < b)
								{
									//Instead of adding new snakes, we're just updating the weights of dead snakes and RESURRECTING them.
									snakes[b].crossover(snakes[0].get_weights(), snakes[1].get_weights());
								}

								snakes[b].update_map();
								snakes[b].reset();
								snakes[b].update_map();
							}
						}

						if (best_score < current_best_score)
						{
							best_score = current_best_score;
						}

						update_game(snakes, map, random_generator);
					}
				}
				else
				{
					update_timer--;
				}
			}

			if (gbl::SCREEN::FRAME_DURATION > lag)
			{
				window.clear();

				for (unsigned char a = 0; a < gbl::MAP::COLUMNS; a++)
				{
					for (unsigned char b = 0; b < gbl::MAP::ROWS; b++)
					{
						if (0 < map[a][b].total_snake_bodies)
						{
							cell_shape.setFillColor(hue_to_rgb(360 * map[a][b].id / static_cast<float>(gbl::GAME::TOTAL_SNAKES)));
						}
						else if (1 == map[a][b].food)
						{
							cell_shape.setFillColor(sf::Color(73, 36, 0));
						}
						else
						{
							cell_shape.setFillColor(sf::Color(0, 0, 0));
						}
						
						cell_shape.setPosition(gbl::MAP::ORIGIN_X + gbl::MAP::CELL_SIZE * a + floor(0.5f * gbl::MAP::CELL_OUTLINE_THICKNESS), gbl::MAP::ORIGIN_Y + gbl::MAP::CELL_SIZE * b + floor(0.5f * gbl::MAP::CELL_OUTLINE_THICKNESS));

						window.draw(cell_shape);
					}
				}

				draw_text(1, 0, 136, 16, "Best length\n" + std::to_string(best_score) + "\n\nCurrent best\nlength\n" + std::to_string(current_best_score) + "\n\nTotal deaths\n" + std::to_string(total_deaths) + "\n\nAverage length\n" + std::to_string(total_length / static_cast<float>(gbl::GAME::TOTAL_SNAKES)), window, font_texture);

				window.display();
			}
		}
	}
}