#include "MessageBus.h"
#include <iostream>
#include <string>

bool MessageBus::send(EventType et, const std::string &info)
{
	bool handled{ false };
	for (std::function<bool(EventType, const std::string &)> callback : m_eventCallbacks)
	{
		handled = callback(et, info);
		if (handled)
		{
			return true;
		}
	}
	return false;
}

bool MessageBus::send(KeyboardInput it, int key)
{
	bool handled{ false };
	for (std::function<bool(KeyboardInput, int)> callback : m_keyCallbacks)
	{
		handled = callback(it, key);
		if (handled)
		{
			return true;
		}
	}
	return false;
}

bool MessageBus::send(MouseInput it, int key, float x, float y)
{
	bool handled{ false };
	for (std::function<bool(MouseInput, int, float, float)> callback : m_mouseCallbacks)
	{
		handled = callback(it, key, x, y);
		if (handled)
		{
			return true;
		}
	}
	return false;
}

bool MessageBus::send(unsigned int codepoint)
{
	bool handled{ false };
	for (std::function<bool(unsigned int)> callback : m_textCallbacks)
	{
		handled = callback(codepoint);
		if (handled)
		{
			return true;
		}
	}
	return false;
}

void MessageBus::registerEventCallback(std::function<bool(EventType, const std::string &)> callback)
{
	m_eventCallbacks.push_back(callback);
	std::cout << "Event callback added\n";
}

void MessageBus::registerKeyboardCallback(std::function<bool(KeyboardInput, int key)> callback)
{
	m_keyCallbacks.push_back(callback);
	std::cout << "Key callback added\n";
}

void MessageBus::registerMouseCallback(std::function<bool(MouseInput, int button, float x, float y)> callback)
{
	m_mouseCallbacks.push_back(callback);
	std::cout << "Key callback added\n";
}

void MessageBus::registerTextCallback(std::function<bool(unsigned int codepoint)> callback)
{
	m_textCallbacks.push_back(callback);
	std::cout << "Text callback added\n";
}

bool MessageBus::deleteEventCallback(std::function<bool(EventType, const std::string &)> callback)
{
	int index{ -1 };
	for (int i{ 0 }; i < m_eventCallbacks.size(); ++i)
	{
		if (false)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		m_eventCallbacks.erase(m_eventCallbacks.begin() + index);
		return true;
	}
	return false;
}

bool MessageBus::deleteKeyboardCallback(std::function<bool(KeyboardInput, int)> callback)
{
	int index{ -1 };
	for (int i{ 0 }; i < m_keyCallbacks.size(); ++i)
	{
		if (false)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		m_keyCallbacks.erase(m_keyCallbacks.begin() + index);
		return true;
	}
	return false;
}

bool MessageBus::deleteMouseCallback(std::function<bool(MouseInput, int, float, float)> callback)
{
	int index{ -1 };
	for (int i{ 0 }; i < m_mouseCallbacks.size(); ++i)
	{
		if (false)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		m_mouseCallbacks.erase(m_mouseCallbacks.begin() + index);
		return true;
	}
	return false;
}

bool MessageBus::deleteTextCallback(std::function<bool(unsigned int)> callback)
{
	int index{ -1 };
	for (int i{ 0 }; i < m_textCallbacks.size(); ++i)
	{
		if (false)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		m_textCallbacks.erase(m_textCallbacks.begin() + index);
		return true;
	}
	return false;
}