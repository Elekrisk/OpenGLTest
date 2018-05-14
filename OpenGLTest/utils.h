#pragma once
#include <random>

// Namespace som inneh�ller funktioner som �r bra att ha
namespace utils
{
	// G�r det m�jligt att mappa tv�dimensionella index till endimensionella och tillbaka, vilket g�r tv�dimensionella arrayer och liknande on�diga.
	int map2DTo1D(int x, int y, int width);
	void map1DTo2D(int index, int width, int &outX, int &outY);
	// F�r ett slumpat heltal
	int getRandomInt(std::mt19937 *generator, int min, int max);
	// Vanlig lerp-funktion. Anv�nds f�r att sammansm�lta bilder.
	float lerp(float t, float a, float b);
	// Genererar ett unikt ID f�r OpenGL-texturer.
	unsigned int generateTextureId();
}