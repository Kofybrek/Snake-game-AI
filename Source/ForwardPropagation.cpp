#include <array>
#include <chrono>
#include <vector>

#include "Headers/ActivationFunction.hpp"
#include "Headers/Global.hpp"

gbl::Vector_1d forward_propagation(const std::array<float, gbl::AI::INPUT_NEURONS>& i_inputs, const gbl::Vector_3d& i_weights)
{
	//input_layer + hidden_layers + output_layer = 2 + hidden_layers
	gbl::Vector_2d neural_network(2 + gbl::AI::HIDDEN_NEURONS.size());
	neural_network[0].resize(gbl::AI::INPUT_NEURONS + gbl::AI::BIAS_NEURONS[0], 0);
	neural_network.back().resize(gbl::AI::OUTPUT_NEURONS, 0);

	for (unsigned char a = 1; a < neural_network.size() - 1; a++)
	{
		neural_network[a].resize(gbl::AI::BIAS_NEURONS[a] + gbl::AI::HIDDEN_NEURONS[a - 1], 0);
	}

	//Putting the inputs in the input layer.
	for (unsigned char a = 0; a < neural_network[0].size(); a++)
	{
		if (a >= gbl::AI::BIAS_NEURONS[0])
		{
			neural_network[0][a] = i_inputs[a - gbl::AI::BIAS_NEURONS[0]];
		}
		else
		{
			neural_network[0][a] = 1;
		}
	}

	for (unsigned char a = 0; a < i_weights.size(); a++)
	{
		unsigned char bias_neurons = 0;

		if (a < i_weights.size() - 1)
		{
			bias_neurons = gbl::AI::BIAS_NEURONS[1 + a];
		}

		std::fill(neural_network[1 + a].begin(), neural_network[1 + a].end(), 1);

		for (unsigned char b = 0; b < i_weights[a].size(); b++)
		{
			neural_network[1 + a][b + bias_neurons] = 0;

			for (unsigned char c = 0; c < i_weights[a][b].size(); c++)
			{
				neural_network[1 + a][b + bias_neurons] += neural_network[a][c] * i_weights[a][b][c];
			}

			//I commented this because for some reason, the snakes don't evolve when I use the activation function.
			//You can uncomment it.
			//If you want, of course.
			//I want it though.
			//neural_network[1 + a][b + bias_neurons] = activation_function(neural_network[1 + a][b + bias_neurons]);
		}
	}

	return neural_network.back();
}