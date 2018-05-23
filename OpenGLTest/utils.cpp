#include "utils.h"
#include <random>

int utils::map2DTo1D(int x, int y, int width)
{
	// Mappar 2d-v�rden till 1D-v�rden
	return x + y * width;
}

void utils::map1DTo2D(int index, int width, int &outX, int &outY)
{
	// Samma som ovan, men �t andra h�llet
	outX = index % width;
	outY = index / width;
}

int utils::getRandomInt(std::mt19937 *generator, int min, int max)
{
	// Skapar ett v�rde, som �r ett delat med det maximala v�rde generatorn kan ge.
	// Detta g�r det enklare att generera v�rden mellan gr�nser.
	double fraction = (1.0 / generator->max());
	// (*generator)() ger ett slupvis heltal mellan 0 och generator->max().
	// Multiplicerat med fraction blir det ett float-v�rde mellan 0 och 1.
	// Multiplicerat med (max - min) blir det mellan 0 och skillnaden mellan min och max, t ex melan 0 och 3 vid min = 7 och max = 10
	// Adderat med min ger r�tt v�rde, som sedan returneras.
	return min + (*generator)() * fraction * (max - min);
}

float utils::lerp(float t, float a, float b)
{
	// Formel tagen fr�n n�gon sida n�gonstan.
	return (1 - t) * a + t * b;
}

unsigned int utils::generateTextureId()
{
	// static, s� att v�rdet sparas.
	static unsigned int ID{ 0 };
	return ID++;
}