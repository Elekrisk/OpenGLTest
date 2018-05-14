#pragma once
#include <glm\glm.hpp>

// Klass som �r bas till alla objekt som finns p� sj�lva banan.
class Drawable
{
public:
	// ID f�r texturen
	unsigned int texture;
	// Position och accessor-funktioner
	int posX, posY;
	int getPosX() const;
	int getPosY() const;
};