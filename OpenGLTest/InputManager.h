#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include "MessageBus.h"

// En struct för en bindning mellan en tangent och ett event
struct Binding
{
	// Koden för tangenten
	int action;
	// Vilket event som ska skickas 
	MessageBus::EventType eventType;
	// Info till eventet
	std::string info;
};

// Hanterar användar-input
namespace InputManager
{
	// Alla kontroller
	extern std::vector<Binding> bindings;
	// Genom vilken bus som meddelanden ska skickas
	extern MessageBus bus;
	// Callbacks som anropas av GLFW
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void textCallback(GLFWwindow* window, unsigned int codepoint);
	// Ställer in default-kontroller
	void setDefaultBindings();
}