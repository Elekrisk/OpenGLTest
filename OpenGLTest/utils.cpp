#include "utils.h"
#include <random>

int utils::map2DTo1D(int x, int y, int width)
{
	// Mappar 2d-värden till 1D-värden
	return x + y * width;
}

void utils::map1DTo2D(int index, int width, int &outX, int &outY)
{
	// Samma som ovan, men åt andra hållet
	outX = index % width;
	outY = index / width;
}

int utils::getRandomInt(std::mt19937 *generator, int min, int max)
{
	// Skapar ett värde, som är ett delat med det maximala värde generatorn kan ge.
	// Detta gör det enklare att generera värden mellan gränser.
	double fraction = (1.0 / generator->max());
	// (*generator)() ger ett slupvis heltal mellan 0 och generator->max().
	// Multiplicerat med fraction blir det ett float-värde mellan 0 och 1.
	// Multiplicerat med (max - min) blir det mellan 0 och skillnaden mellan min och max, t ex melan 0 och 3 vid min = 7 och max = 10
	// Adderat med min ger rätt värde, som sedan returneras.
	return min + (*generator)() * fraction * (max - min);
}

float utils::lerp(float t, float a, float b)
{
	// Formel tagen från någon sida någonstan.
	return (1 - t) * a + t * b;
}

unsigned int utils::generateTextureId()
{
	// static, så att värdet sparas.
	static unsigned int ID{ 0 };
	return ID++;
}