#pragma once
#include "MessageBus.h"
#include <memory>
#include "ImageConstructor.h"

class UIElement;
class UIGroup;

// Klass som hanterar UI:n. Ej helt implementerad �n.
class UI
{
	// Den messageBus som UI:n ska lyssna p�.
	MessageBus *m_messageBus;
	// Skal�r f�r storleken p� UI:n
	float m_uiScale;
	// Alla elementgrupper i UI:n. En elementgrupp �r en grupp UI-element som h�r till varandra och kan g�mmas tillsammans.
	// Detta kan t ex vara ett UI-f�nster, som en inventory-panel.
	std::vector<UIGroup> m_elementGroups;
public:
	// Konstrukt�r som binder UI:n till messageBusen
	UI(MessageBus &messageBus);
	// De olika callbacksen (svengelska :D) som anv�nds av UI:n.
	bool eventCallback(MessageBus::EventType et, const std::string &info);
	bool keyboardCallback(MessageBus::KeyboardInput it, int key);
	bool textCallback(unsigned int codepoint);
	bool mouseCallback(MessageBus::MouseInput it, int key, int x, int y);
	// Funktion som returnerar alla elementgrupper
	std::vector<UIGroup> *getElementGroups();

	// Enumeration f�r hur ett f�nster ska reagera p� olika sk�rmuppl�sningar.
	enum AnchorPoint
	{
		UPPER_LEFT,
		UPPER_RIGHT,
		LOWER_LEFT,
		LOWER_RIGHT,
		EDGE_LEFT,
		EDGE_RIGHT,
		EDGE_UP,
		EDGE_DOWN
	};
};

// En klass som representerar en UI-elementgrupp.
class UIGroup
{
	// Ifall gruppen ska renderas och reagera p� interaktioner.
	bool m_enabled{ false };
	// Var den renderas i z-led, vilket best�mmer ifall den kommer renderas framf�r eller bakom andra grupper.
	int m_zIndex;
	// Alla element som ing�r i denna grupp.
	std::vector<std::unique_ptr<UIElement>> m_elements;
	// Dess anchor, som beskrivs i UI-klassen
	UI::AnchorPoint m_anchor;
	
	// J�mf�r z-index.
	bool operator<(const UIGroup &group) const;
public:
	// Funktioner f�r att s�tta p� eller st�nga av gruppen.
	void enable();
	void disable();
	// Best�mmer z-index och returnerar z-index respektivt.
	void setZIndex(int index);
	int getZIndex();
	// Returnerar en pekare till en vector med alla elementen i gruppen.
	std::vector<std::unique_ptr<UIElement>> *getElements();
	// Renturnerar och best�mmer gruppens anchor.
	UI::AnchorPoint getAnchor();
	void setAnchor(UI::AnchorPoint);
};

// En klass som representerar ett UI-element
class UIElement
{
protected:
	// Dess position, storlek och z-index
	float m_posX;
	float m_posY;
	int m_width;
	int m_height;
	int m_zIndex;

	// Ifall UI-elementet har uppdaterats eller om det m�ste byggas om
	bool m_refreshed{ false };
	// Elementets OpenGL-textur
	unsigned int m_texture;
	// Bygg elementet. Jag �r os�ker p� om jag kommer att anv�nda denna funktion eller enbart anv�nda refresh()
	void construct();
public:
	UIElement();
	// S�tter m_refreshed = true
	bool shouldRefresh();
	// Bygg om elementet.
	void refresh();
};

// En klass som representerar en bild, som huvudsakligt kommer att anv�ndas som bakgrund till andra element.
class UIBackground : public UIElement
{
protected:
	// Den bild som bakgrunden anv�nder
	Image m_img;

	void construct();
public:
	UIBackground();
	// Returnerar bilden och best�mmer bilden.
	const Image *getImage();
	void setImage(const Image &img);
	void refresh();
};

// En klass som representerar text.
class UIText : public UIElement
{
	// Den text som kommer att renderas till en bild.
	std::string m_text;
	// Den bild som renderingen spottar ut
	Image m_imgCompiled;

	void construct();
public:
	UIText();
	// Returnerar texten och best�mmer texten
	const std::string *getText();
	void setText(const std::string &text);
	// Returnerar den kompilerade bilden
	const Image *getImage();
	void refresh();
};

// En klass som representerar en knapp.
class UIButton : public UIElement
{
protected:
	// Den funktion som anropas n�r knappen aktiveras.
	std::function<int(int id)> m_function;

	void construct();
public:
	UIButton();
	// Best�mmer respektive returnerar funktionen.
	void setFunction(std::function<int(int id)> function);
	std::function<int(int id)> getFunction();
	// Aktivera knappen.
	int activate(int id);
};

// En knapp som har bilder som representerar dess tilst�nd.
class UIImageButton : public UIButton
{
protected:
	// Bilder som visas n�r knappen �r normal, blir hovrad �ver respektive �r nedtryckt.
	Image m_normalImg;
	Image m_hoverImg;
	Image m_clickedImg;
	// Vilket tillst�nd knappen �r i
	int m_state;

	void construct();
public:
	UIImageButton();
	// Returnerar respektive best�mmer bilderna
	void setImage(int imageState, const Image &image);
	const Image *getImage(int imageState);
	// Best�mmer respektive returnerar knappens tillst�nd
	void setState(int imageState);
	int getState();
};

// Samma som ovan plus text
class UITextButton : public UIImageButton
{
protected:
	std::string m_text;
	// Kompilerade bilder
	Image m_normalImgCompiled;
	Image m_hoverImgCompiled;
	Image m_clickedImgCompiled;

	void construct();
public:
	UITextButton();
	const Image *getCompiledImage(int imageState);
	void setText(const std::string &text);
	const std::string *getText();
};

// Representerar ett redigerbart textf�lt
class UIField : public UIElement
{
protected:
	// Bakgrundsbilder f�r texten, och sj�lva texten
	Image m_normalImg;
	Image m_selectedImg;
	std::string m_text;
	// Ifall f�ltet har uppdateras sedan anv�ndaren redigerade texten.
	bool m_updated;

	// De kompilerade bilderna.
	Image m_normalImgCompiled;
	Image m_selectedImgCompiled;

	void construct();
public:
	UIField();
	void setImage(int imageState, const Image &image);
	const Image *getImage(int imageState);
	const Image *getCompiledImage(int imageState);
};