#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include "shaderclass.h"
#include "ImageConstructor.h"
#include "stbi_stuff.h"
#include "Drawable.h"
#include <random>
#include "Map.h"
#include "MessageBus.h"
#include "ui.h"
#include "Renderer.h"
#include "Camera.h"
#include "InputManager.h"
#include "Main.h"

// Initierar tv� globala variabler som inneh�ller f�nstrets storlek. Anv�nds bl a av Renderer
float windowHeight{ 600 };
float windowWidth{ 800 };

// De objekt som ska renderas p� kartan.
std::vector<Drawable*> drawables{ };
// Kameran som anv�nds f�r att rendera. Inneh�ller position, zoom och dess matris
Camera camera;

// Aktiveras n�r spelaren �ndrar zoom-niv�
bool zoomCallback(MessageBus::EventType et, const std::string &info)
{
	if (et == MessageBus::EVENT_ZOOM)
	{
		switch (info[0])
		{
			case '-':
				camera.zoom /= 1.15f;
				break;
			case '+':
				camera.zoom *= 1.15f;
				break;
			default:
				break;
		}
	}
	return false;
}

// Funktionen k�rs n�r programmet startar
int main()
{
	// Initierar klasser.
	// MessageBus hanterar skickandet av meddelande mellan moduler
	MessageBus bus;
	// UI hanterar anv�ndargr�nssnittet
	UI ui(bus);
	// Klass som representerar f�nstret; kommer fr�n det externa biblioteket GLFW, vilket hanterar f�nstret
	GLFWwindow* window;
	// Klass som hanterar rendering
	Renderer renderer;
	// Initiera renderaren, vilket ocks� startar ett GLFW-f�nster
	renderer.init(windowWidth, windowHeight, "Testing Geemu", window);

	// Klass som representerar den bana som spelet spelas p�. Genererar just nu en testbana som �r 100x100 rutor stor
	Map map;
	map.generate(100, 100);

	// En klass som hanterar ett par av OpenGL-shaders.
	Shader ourShader("vertexShader.vert", "fragmentShader.frag");

	// Skapar spelaren. Detta ska senare vara implementerat i en klass
	int width, height, nrChannels;
	// Anv�nder biblioteket stbi f�r att ladda in en PNG-bild i minnet. "stbi"-namespacet �r mitt eget, d� stbi �r ett c-bibliotek och d�rmed har globala funktioner
	unsigned char* data = stbi::loadImage("player.png", width, height, nrChannels);
	// Funktionen returnerar en pekare till b�rjan p� bilddatan, eller -1 ifall den misslyckades. I s� fall, avsluta programmet.
	// I framtiden ska jag troligtvis implementera felmeddelanden f�r detta.
	if (!data)
	{
		return -1;
	}
	// Generera ett unikt ID som beh�vs f�r OpenGL:s texturer
	unsigned int playerTexture{ utils::generateTextureId() };
	// Koden nedan skapar en textur och l�ser in bilddatan till den texturen
	glGenTextures(1, &playerTexture);
	glBindTexture(GL_TEXTURE_2D, playerTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Frig�r bilddatan fr�n minnet d� den ej beh�vs l�ngre
	stbi::freeImage(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Skapar en ny spelare, och assignar den till min MessageBus. Detta ska d� ocks� abstraheras bort.
	Player* p = new Player(bus);
	p->texture = playerTexture;
	p->posX = 1;
	p->posY = 1;
	// L�gger till spelaren till Drawables, som kommer att ritas p� banan
	drawables.push_back(p);

	// L�gger till en callback i MessageBusen f�r zoomning.
	bus.registerEventCallback(&zoomCallback);

	// D� jag just nu sparar en kopia i namespacet InputManager m�ste denna assignering ske efter att bussen har modifierats klart.
	InputManager::bus = bus;
	// S�tter n�gra callbacks som ges av GLFW till funktioner i InputManager
  	glfwSetKeyCallback(window, &InputManager::keyCallback);
	glfwSetCharCallback(window, InputManager::textCallback);
	// St�ller in default-kontroller p� InputManager. Utan detta skulle inga kontroller fungera.
	InputManager::setDefaultBindings();

	// glfwWindwShouldClose(GFLWwindow*) �r en funktion som kollar ifall anv�ndaren har beg�rt att f�nstret ska st�ngas ner, t ex genom att
	// klicka p� krysset uppe till h�ger p� f�nstret. Detta kan ocks� beg�ras av programmet sj�lv.
	while (!glfwWindowShouldClose(window))
	{		 
		// G�r s� att kameran f�ljer efter spelaren
		camera.posX = -p->posX;
		camera.posY = -p->posY;

		// Renderar allting
		renderer.render(map, drawables, camera, ourShader, window);

		// GLFW-funktion som h�mtar anv�ndar-input och anropar respektive callback
		glfwPollEvents();
	}
	// Avslutar GLFW
	glfwTerminate();
	// Frig�r alla Drawables i drawables
	for (Drawable *d : drawables)
	{
		delete d;
	}
	// Avslutar programmet.
	return 0;
}