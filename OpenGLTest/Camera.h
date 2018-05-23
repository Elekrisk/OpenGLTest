#pragma once
#include <glm\glm.hpp>

// Denna fil har ingen .cpp-motsvarighet.
class Camera
{
public:
	// Position och zoom
	float m_posX{ 0.0f };
	float m_posY{ 0.0f };
	float m_zoom{ 1.0f};
	// En matris som inehåller samma värden som ovan, men i ett format som enkelt kan användas för att manipulera grafiken
	glm::mat4 m_projection;
};