#pragma once
#include <glm\glm.hpp>

// Klass som �r bas till alla objekt som finns p� sj�lva banan.
class Drawable
{
public:
	// ID f�r texturen
	unsigned int m_texture;
	// Position och accessor-funktioner
	int m_posX, m_posY;
	int getPosX() const;
	int getPosY() const;
};