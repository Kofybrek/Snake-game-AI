#pragma once

class RandomGenerator
{
	std::mt19937_64 random_engine;
public:
	RandomGenerator();

	//I wanted to make 1 function that checks if the value_type is integer or not and generate random numbers based on that. But I couldn't make it work.
	template <typename value_type>
	value_type get_random_int(const value_type i_max_value = std::numeric_limits<value_type>::max(), const value_type i_min_value = std::numeric_limits<value_type>::min())
	{
		std::uniform_int_distribution<value_type> distribution(i_min_value, i_max_value);

		return distribution(random_engine);
	}

	template <typename value_type>
	value_type get_random_real(const value_type i_max_value = std::numeric_limits<value_type>::max(), const value_type i_min_value = std::numeric_limits<value_type>::min())
	{
		//As you can see, I'm using std::nextafter(). That's because the uniform_real_distribution includes the min value but doesn't include the max value. And I'm to much of a crazy perfectionist to let it be.
		std::uniform_real_distribution<value_type> distribution(i_min_value, std::nextafter(i_max_value, std::numeric_limits<value_type>::max()));

		return distribution(random_engine);
	}
};