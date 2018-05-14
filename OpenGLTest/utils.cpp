#include "utils.h"
#include <random>

int utils::map2DTo1D(int x, int y, int width)
{
	return x + y * width;
}

void utils::map1DTo2D(int index, int width, int &outX, int &outY)
{
	outX = index % width;
	outY = index / width;
}

int utils::getRandomInt(std::mt19937 *generator, int min, int max)
{
	double fraction = (1.0 / generator->max());
	return min + (*generator)() * fraction * (max - min);
}

float utils::lerp(float t, float a, float b)
{
	return (1 - t) * a + t * b;
}

unsigned int utils::generateTextureId()
{
	static unsigned int ID{ 0 };
	return ID++;
}