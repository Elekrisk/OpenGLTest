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
	// Registrerar alla callbacks. Om ett textfält är markerat ska inga events komma igenom oskadda.
	m_messageBus = &messageBus;
	messageBus.registerEventCallback(std::bind(&UI::eventCallback, this, std::placeholders::_1, std::placeholders::_2));
	messageBus.registerKeyboardCallback(std::bind(&UI::keyboardCallback, this, std::placeholders::_1, std::placeholders::_2));
	messageBus.registerTextCallback(std::bind(&UI::textCallback, this, std::placeholders::_1));
	messageBus.registerMouseCallback(std::bind(&UI::mouseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	
	// Skapar en UIGroup som ska representera huvudmenyn. Används ej än.
	UIGroup mainMenu();
}

// Alla callbacks returnerar just nu false, och påverkar då ingenting, då UI ej används än.
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
// Returnerar alla element i en grupp.
std::vector<UIGroup> *UI::getElementGroups()
{
	return &m_elementGroups;
}



// Jämför z-index när man jämför grupper direkt
// Jag har kommenterat alla funktioner i header-filen, och gör det bara igen ifall det finns något till att kommentera.
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

// Assignar ett ID till sin textur.
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

// Ifall datan i bilden finns, generera en ny textur.
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

// Början på en refresh-metod som ska kompilera bilden från en bakgrundsbild plus text.
void UIText::refresh()
{
	// Den mask som text ska skrivas till. Är just nu tom.
	Image textMask;//ImageConstructor::createTextMask(m_text, 16);
	// Gör en char-array med samma storlek som bildens data och fyller den med värdet 0
	unsigned char *hello = new unsigned char[textMask.getWidth() * textMask.getHeight() * 4]{ 0 };
	// Skapar en ny bild med datan ovanför och lägger in den i m_imgCompiled, vilket är den bild som sedan ska ritas.
	m_imgCompiled = Image(hello, textMask.getWidth(), textMask.getHeight(), 4);
	// Skapar den färg som texten ska vara, just nu vit.
	std::vector<unsigned char> colors{ 255, 255, 255, 255};
	Color white(colors);
	// Använder masken för att färga den kompilerade bilden vit.
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

// Använder en switch för att returnera rätt bild
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