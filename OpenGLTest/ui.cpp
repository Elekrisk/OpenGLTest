#include "ui.h"
#include "MessageBus.h"
#include <iostream>
#include <string>
#include <functional>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "ImageConstructor.h"


UI::UI(MessageBus &messageBus)
{
	m_messageBus = &messageBus;
	messageBus.registerEventCallback(std::bind(&UI::eventCallback, this, std::placeholders::_1, std::placeholders::_2));
	messageBus.registerKeyboardCallback(std::bind(&UI::keyboardCallback, this, std::placeholders::_1, std::placeholders::_2));
	messageBus.registerTextCallback(std::bind(&UI::textCallback, this, std::placeholders::_1));
	messageBus.registerMouseCallback(std::bind(&UI::mouseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	
	UIGroup mainMenu();
}

bool UI::eventCallback(MessageBus::EventType et, const std::string &info)
{
	return false;
}

bool UI::keyboardCallback(MessageBus::KeyboardInput it, int key)
{
	return false;
}

bool UI::textCallback(unsigned int codepoint)
{
	return false;
}

bool UI::mouseCallback(MessageBus::MouseInput mi, int key, int x, int y)
{
	return false;
}

std::vector<UIGroup> *UI::getElementGroups()
{
	return &m_elementGroups;
}



bool UIGroup::operator<(const UIGroup &group) const
{
	return m_zIndex < group.m_zIndex;
}

void UIGroup::enable()
{
	m_enabled = true;
}

void UIGroup::disable()
{
	m_enabled = false;
}

void UIGroup::setZIndex(int index)
{
	m_zIndex = index;
}

int UIGroup::getZIndex()
{
	return m_zIndex;
}

std::vector<std::unique_ptr<UIElement>> *UIGroup::getElements()
{
	return &m_elements;
}

UI::AnchorPoint UIGroup::getAnchor()
{
	return m_anchor;
}

void UIGroup::setAnchor(UI::AnchorPoint anchor)
{
	m_anchor = anchor;
}



UIElement::UIElement()
{
	construct();
}

void UIElement::construct()
{
	m_texture = utils::generateTextureId();
}

bool UIElement::shouldRefresh()
{
	return !m_refreshed;
}

void UIElement::refresh()
{
	m_refreshed = true;
}



UIBackground::UIBackground() : UIElement()
{

}

void UIBackground::construct()
{
	
}

const Image *UIBackground::getImage()
{
	return &m_img;
}

void UIBackground::setImage(const Image &img)
{
	m_img = img;
}

void UIBackground::refresh()
{
	if (m_img.getData() != nullptr)
	{
		glGenTextures(1, &m_texture);
	}
}



UIText::UIText() : UIElement()
{
	construct();
}

void UIText::construct()
{
	
}

const std::string *UIText::getText()
{
	return &m_text;
}

void UIText::setText(const std::string &text)
{
	m_text = text;
}

const Image *UIText::getImage()
{
	return &m_imgCompiled;
}

void UIText::refresh()
{
	Image textMask;//ImageConstructor::createTextMask(m_text, 16);
	unsigned char *hello = new unsigned char[textMask.getWidth() * textMask.getHeight() * 4]{ 0 };
	m_imgCompiled = Image(hello, textMask.getWidth(), textMask.getHeight(), 4);
	std::vector<unsigned char> colors{ 255, 255, 255, 255};
	Color white(colors);
	ImageConstructor::composite(&m_imgCompiled, &textMask, white, 0, 0);
}



UIButton::UIButton() : UIElement()
{
	construct();
}

void UIButton::construct()
{
	
}

void UIButton::setFunction(std::function<int(int id)> function)
{
	m_function = function;
}

std::function<int(int id)> UIButton::getFunction()
{
	return m_function;
}

int UIButton::activate(int id)
{
	return m_function(id);
}



UIImageButton::UIImageButton() : UIButton()
{
	construct();
}

void UIImageButton::construct()
{
	
}

void UIImageButton::setImage(int imageState, const Image &img)
{
	switch (imageState)
	{
		case 0:
			m_normalImg = img;
			break;
		case 1:
			m_hoverImg = img;
			break;
		case 2:
			m_clickedImg = img;
			break;
		default:
			break;
	}
}

const Image *UIImageButton::getImage(int imageState)
{
	switch (imageState)
	{
		case 0:
			return &m_normalImg;
		case 1:
			return &m_hoverImg;
		case 2:
			return &m_clickedImg;
		default:
			break;
	}
}

void UIImageButton::setState(int imageState)
{
	m_state = imageState;
}

int UIImageButton::getState()
{
	return m_state;
}



UITextButton::UITextButton() : UIImageButton()
{
	construct();
}

void UITextButton::construct()
{
	
}

const Image *UITextButton::getCompiledImage(int imageState)
{
	switch (imageState)
	{
		case 0:
			return &m_normalImgCompiled;
		case 1:
			return &m_hoverImgCompiled;
		case 2:
			return &m_clickedImgCompiled;
		default:
			break;
	}
}

void UITextButton::setText(const std::string &text)
{
	m_text = text;
}

const std::string *UITextButton::getText()
{
	return &m_text;
}



UIField::UIField() : UIElement()
{
	construct();
}

void UIField::construct()
{

}

void UIField::setImage(int imageState, const Image &img)
{
	switch (imageState)
	{
		case 0:
			m_normalImg = img;
			break;
		case 1:
			m_selectedImg = img;
			break;
		default:
			break;
	}
}

const Image *UIField::getImage(int imageState)
{
	switch (imageState)
	{
		case 0:
			return &m_normalImg;
		case 1:
			return &m_selectedImg;
		default:
			break;
	}
}

const Image *UIField::getCompiledImage(int imageState)
{
	switch (imageState)
	{
		case 0:
			return &m_normalImgCompiled;
		case 1:
			return &m_selectedImgCompiled;
		default:
			break;
	}
}