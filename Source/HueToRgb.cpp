#include <SFML/Graphics.hpp>

//Oh, you're wondering how this AMAZING code that I wrote ENTIRELY BY MYSELF without using ANY HELP like Stackoverflow works?
//This code is SOOOO AMAZING that I don't even have to explain it to you.
//Just keep looking at it until you understand it.

sf::Color hue_to_rgb(const float i_hue)
{
	unsigned char hue_color = round(255 * (1 - abs(fmod(i_hue / 60.f, 2) - 1)));

	sf::Color output_color;

	if (60 > i_hue)
	{
		output_color.b = 0;
		output_color.g = hue_color;
		output_color.r = 255;
	}
	else if (120 > i_hue)
	{
		output_color.b = 0;
		output_color.g = 255;
		output_color.r = hue_color;
	}
	else if (180 > i_hue)
	{
		output_color.b = hue_color;
		output_color.g = 255;
		output_color.r = 0;
	}
	else if (240 > i_hue)
	{
		output_color.b = 255;
		output_color.g = hue_color;
		output_color.r = 0;
	}
	else if (300 > i_hue)
	{
		output_color.b = 255;
		output_color.g = 0;
		output_color.r = hue_color;
	}
	else
	{
		output_color.b = hue_color;
		output_color.g = 0;
		output_color.r = 255;
	}

	return output_color;
}