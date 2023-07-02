#include <chrono>
#include <random>

#include "Headers/RandomGenerator.hpp"

RandomGenerator::RandomGenerator() :
	random_engine(std::chrono::system_clock::now().time_since_epoch().count())
{

}