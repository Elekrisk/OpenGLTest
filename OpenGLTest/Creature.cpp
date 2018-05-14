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
	std::cout << "TEST\n";
	// Just nu reagerar spelaren endast p� MOVE-event, och det �r dessutom det enda event f�rutom ZOOM
	if (et == MessageBus::EVENT_MOVE)
	{
		// En l�ng if-else kedja d� switch ej fungerar f�r en string d� det ej �r en fundamental datatyp.
		// Flytta spelaren i den riktning som meddelandet s�ger.
		if (info == "NW")
		{
			--posX;
			++posY;
		}
		else if (info == "N")
		{
			++posY;
		}
		else if (info == "NE")
		{
			++posY;
			++posX;
		}
		else if (info == "E")
		{
			++posX;
		}
		else if (info == "SE")
		{
			++posX;
			--posY;
		}
		else if (info == "S")
		{
			--posY;
		}
		else if (info == "SW")
		{
			--posY;
			--posX;
		}
		else if (info == "W")
		{
			--posX;
		}
		return true;
	}
	// Returnerar false ifall eventet inte var ett MOVE-event, vilket till�ter eventet att f�rdas nedf�r kedjan.
	return false;
}