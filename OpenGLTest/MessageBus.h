#pragma once
#include <vector>
#include <functional>

// Klass för att hantera skickandet av meddelanden mellan olika moduler.
class MessageBus
{
public:
	// Typ av events som kan skickas. Just nu finns det bara MOVE och ZOOM.
	enum EventType
	{
		EVENT_MOVE,
		EVENT_ZOOM
	};

	// Vad för handling som sker i och med en tangenttryckning
	enum KeyboardInput
	{
		KEY_PRESSED,
		KEY_HELD,
		KEY_RELEASED
	};

	// Eventtyper för musen
	enum MouseInput
	{
		MOUSE_PRESSED,
		MOUSE_RELEASED,
		MOUSE_HAS_MOVED
	};

private:
	// Vectors som innehåller alla callbacks som ska anropas när respektive send()-metod anropas
	std::vector<std::function<bool(EventType mt, const std::string &info)>> m_eventCallbacks;
	std::vector<std::function<bool(KeyboardInput ki, int key)>> m_keyCallbacks;
	std::vector<std::function<bool(MouseInput mi, int key, float x, float y)>> m_mouseCallbacks;
	std::vector<std::function<bool(const unsigned int codepoint)>> m_textCallbacks;

public:
	// Anropar respektive callback med de givna argument
	bool send(EventType mt, const std::string &info);
	bool send(KeyboardInput mt, int key);
	bool send(MouseInput mt, int button, float x, float y);
	bool send(unsigned int codepoint);
	// Registrerar en callback av respektive typ.
	void registerEventCallback(std::function<bool(EventType, const std::string &)> callback);
	void registerKeyboardCallback(std::function<bool(KeyboardInput, int)> callback);
	void registerMouseCallback(std::function<bool(MouseInput, int, float, float)> callback);
	void registerTextCallback(std::function<bool(unsigned int)> callback);
	// Tar bort en callback av respektive typ.
	bool deleteEventCallback(std::function<bool(EventType, const std::string &)> callback);
	bool deleteKeyboardCallback(std::function<bool(KeyboardInput, int)> callback);
	bool deleteMouseCallback(std::function<bool(MouseInput, int, float, float)> callback);
	bool deleteTextCallback(std::function<bool(unsigned int)> callback);
};