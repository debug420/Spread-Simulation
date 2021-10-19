#pragma once
#include <random>

int random(int min, int max)
{
	std::default_random_engine engine(std::random_device{}());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine);
}