#include "../include/wired/wRandom.h"

#include <stdlib.h>

void wRandomSetSeed(unsigned seed)
{
	srand(seed);
}

int wRandomInt(int min, int max)
{
	if (max < min) {
		int tmp = max;
		max = min;
		min = max;
	}

	int range = max - min;
	return min + (rand() % range);
}

float wRandomFloat(float min, float max)
{
	if (max < min) {
		float tmp = max;
		max = min;
		min = max;
	}

	return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}
