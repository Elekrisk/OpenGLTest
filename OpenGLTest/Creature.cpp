#include "Creature.h"
#include <functional>
#include <iostream>

Player::Player(MessageBus &bus)
{
	// Registrerar callbacken f�r events
	bus.registerEventCallback(std::bind(&Player::eventCallback, this, std::placeholders::_1, std::placeholders::_2));
}

bool Player::eventCallback(MessageBus::EventType et, const std::string &info)
{
	//std::cout << "TEST\n";
	// Just nu reagerar spelaren endast p� MOVE-event, och det �r dessutom det enda event f�rutom ZOOM
	if (et == MessageBus::EVENT_MOVE)
	{
		// En l�ng if-else kedja d� switch ej fungerar f�r en string d� det ej �r en fundamental datatyp.
		// Flytta spelaren i den riktning som meddelandet s�ger.
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
	// Returnerar false ifall eventet inte var ett MOVE-event, vilket till�ter eventet att f�rdas nedf�r kedjan.
	return false;
}