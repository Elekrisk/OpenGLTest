#pragma once
#include <glm\glm.hpp>

// Klass som är bas till alla objekt som finns på själva banan.
class Drawable
{
public:
	// ID för texturen
	unsigned int m_texture;
	// Position och accessor-funktioner
	int m_posX, m_posY;
	int getPosX() const;
	int getPosY() const;
};