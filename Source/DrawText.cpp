#include <SFML/Graphics.hpp>
#include <cmath>

void draw_text(const bool i_center_x, const bool i_center_y, const short i_x, const short i_y, const std::string& i_text, sf::RenderWindow& i_window, const sf::Texture& i_font_texture)
{
	short character_x;
	short character_y;

	//Our font texture consists of 96 characters.
	unsigned char character_width = i_font_texture.getSize().x / 96;

	sf::Sprite character_sprite(i_font_texture);

	if (1 == i_center_x)
	{
		character_x = static_cast<short>(round(i_x - 0.5f * character_width * i_text.substr(0, i_text.find_first_of('\n')).size()));
	}
	else
	{
		character_x = i_x;
	}

	if (1 == i_center_y)
	{
		character_y = static_cast<short>(round(i_y - 0.5f * i_font_texture.getSize().y * (1 + std::count(i_text.begin(), i_text.end(), '\n'))));
	}
	else
	{
		character_y = i_y;
	}

	for (unsigned short a = 0; a < i_text.size(); a++)
	{
		if ('\n' == i_text[a])
		{
			character_y += i_font_texture.getSize().y;

			if (1 == i_center_x)
			{
				//Oh, you wanna know how this code works? Me too.
				character_x = static_cast<short>(round(i_x - 0.5f * character_width * i_text.substr(1 + a, i_text.find_first_of('\n', 1 + a) - 1 - a).size()));
			}
			else
			{
				character_x = i_x;
			}

			continue;
		}

		character_sprite.setPosition(character_x, character_y);
		character_sprite.setTextureRect(sf::IntRect(character_width * (i_text[a] - 32), 0, character_width, i_font_texture.getSize().y));

		character_x += character_width;

		i_window.draw(character_sprite);
	}
}