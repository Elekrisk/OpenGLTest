#pragma once
#include <glm\glm.hpp>

// Denna fil har ingen .cpp-motsvarighet.
class Camera
{
public:
	// Position och zoom
	float posX{ 0.0f };
	float posY{ 0.0f };
	float zoom{ 1.0f};
	// En matris som inehåller samma värden som ovan, men i ett format som enkelt kan användas för att manipulera grafiken
	glm::mat4 projection;
};