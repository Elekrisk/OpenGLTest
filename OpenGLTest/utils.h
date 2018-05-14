#pragma once
#include <random>

// Namespace som innehåller funktioner som är bra att ha
namespace utils
{
	// Gör det möjligt att mappa tvådimensionella index till endimensionella och tillbaka, vilket gör tvådimensionella arrayer och liknande onödiga.
	int map2DTo1D(int x, int y, int width);
	void map1DTo2D(int index, int width, int &outX, int &outY);
	// Får ett slumpat heltal
	int getRandomInt(std::mt19937 *generator, int min, int max);
	// Vanlig lerp-funktion. Används för att sammansmälta bilder.
	float lerp(float t, float a, float b);
	// Genererar ett unikt ID för OpenGL-texturer.
	unsigned int generateTextureId();
}