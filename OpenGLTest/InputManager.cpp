#include "InputManager.h"
#include <string>
#include <vector>
#include <iostream>

// Kontrollerna som är bundna
std::vector<Binding> InputManager::bindings{ };
// Den MessageBus som input ska skickas via
MessageBus InputManager::bus;

// Callback från GLFW som anropas med om en tangent trycks ned. Tangenten skickas med modifiers separat.
void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sätter default-index till -1 då detta kan indikera ifall en binding hittades eller inte, då ett index ej kan vara negativ.
	int actionIndex{ -1 };
	// Leta efter tangentkoden i de bundna kontrollerna.
	for (int i{ 0 }; i < bindings.size(); ++i)
	{
		if (bindings[i].action == key)
		{
			actionIndex = i;
			break;
		}
	}
	// DEBUG
	std::cout << "\n" << actionIndex << "\n";

	// Ifall ingen binding hittades för den nedtryckta knappen
	if (actionIndex == -1)
	{
		// Skickar tangentkoden via en av callbackerna med respektive 'action'
		switch (action)
		{
			case GLFW_PRESS:
				bus.send(bus.KEY_PRESSED, key);
				break;
			case GLFW_REPEAT:
				bus.send(bus.KEY_HELD, key);
				break;
			case GLFW_RELEASE:
				bus.send(bus.KEY_RELEASED, key);
				break;
			default:
				break;
		}
	}
	// Ifall en binding hittades och detta är den första framen som tangenten trycks ned
	else if (action == GLFW_PRESS)
	{
		// Skicka det event som tangenten är bunden till, tillsammans med information från bindningen
		bus.send(bindings[actionIndex].eventType, bindings[actionIndex].info);
	}
}

// Callback från GLFW som anropas med en Unicode-karaktär, den som ett textfält skulle visa.
void InputManager::textCallback(GLFWwindow* window, unsigned int codepoint)
{
	bus.send(codepoint);
}

void InputManager::setDefaultBindings()
{
	// Reservera elva platser i vectorn
	bindings.reserve(11);
	// Registrerar alla bindningar.
	Binding t1{ 81, MessageBus::EVENT_MOVE, "NW" };
	bindings.push_back(t1);
	Binding t2{ 87, MessageBus::EVENT_MOVE, "N" };
	bindings.push_back(t2);
	Binding t3{ 69, MessageBus::EVENT_MOVE, "NE" };
	bindings.push_back(t3);
	Binding t4{ 65, MessageBus::EVENT_MOVE, "W" };
	bindings.push_back(t4);
	Binding t5{ 68, MessageBus::EVENT_MOVE, "E" };
	bindings.push_back(t5);
	Binding t6{ 90, MessageBus::EVENT_MOVE, "SW" };
	bindings.push_back(t6);
	Binding t7{ 88, MessageBus::EVENT_MOVE, "S" };
	bindings.push_back(t7);
	Binding t8{ 67, MessageBus::EVENT_MOVE, "SE" };
	bindings.push_back(t8);
	Binding t9{ 83, MessageBus::EVENT_MOVE, "0" };
	bindings.push_back(t9);
	Binding t10{ 333, MessageBus::EVENT_ZOOM, "-" };
	bindings.push_back(t10);
	Binding t11{ 334, MessageBus::EVENT_ZOOM, "+" };
	bindings.push_back(t11);
	// Skriv ut alla bindningar till terminalen
	for (int i{ 0 }; i < bindings.size(); ++i)
	{
		std::cout << bindings[i].action << " -- " << bindings[i].info;
	}
}