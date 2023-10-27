#include <algorithm>
#include <cmath>

float activation_function(const float i_value)
{
	if (0 >= i_value)
	{
		return powf(2, i_value - 1);
	}
	else
	{
		return 1 - powf(2, -1 - i_value);
	}
}