#include "Creature.h"
#include <functional>
#include <iostream>

Player::Player(MessageBus &bus)
{
	// Registrerar callbacken för events
	bus.registerEventCallback(std::bind(&Player::eventCallback, this, std::placeholders::_1, std::placeholders::_2));
}

bool Player::eventCallback(MessageBus::EventType et, const std::string &info)
{
	//std::cout << "TEST\n";
	// Just nu reagerar spelaren endast på MOVE-event, och det är dessutom det enda event förutom ZOOM
	if (et == MessageBus::EVENT_MOVE)
	{
		// En lång if-else kedja då switch ej fungerar för en string då det ej är en fundamental datatyp.
		// Flytta spelaren i den riktning som meddelandet säger.
		if (info == "NW")
		{
			--m_posX;
			++m_posY;
		}
		else if (info == "N")
		{
			++m_posY;
		}
		else if (info == "NE")
		{
			++m_posY;
			++m_posX;
		}
		else if (info == "E")
		{
			++m_posX;
		}
		else if (info == "SE")
		{
			++m_posX;
			--m_posY;
		}
		else if (info == "S")
		{
			--m_posY;
		}
		else if (info == "SW")
		{
			--m_posY;
			--m_posX;
		}
		else if (info == "W")
		{
			--m_posX;
		}
		return true;
	}
	// Returnerar false ifall eventet inte var ett MOVE-event, vilket tillåter eventet att färdas nedför kedjan.
	return false;
}