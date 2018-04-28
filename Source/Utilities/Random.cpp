#include "Random.h"
#include <cstdlib>

// Definitions of the random functions
float Random()
{
	return rand() / static_cast<float>(RAND_MAX);
}

float Random(const float min, const float max)
{
	return min + Random() * (max - min);
}

int RandomInt(const int min, const int max)
{
	return static_cast<int>(Random() * (max - min)) + min;
}