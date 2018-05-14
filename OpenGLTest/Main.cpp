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

// Initierar två globala variabler som innehåller fönstrets storlek. Används bl a av Renderer
float windowHeight{ 600 };
float windowWidth{ 800 };

// De objekt som ska renderas på kartan.
std::vector<Drawable*> drawables{ };
// Kameran som används för att rendera. Innehåller position, zoom och dess matris
Camera camera;

// Aktiveras när spelaren ändrar zoom-nivå
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

// Funktionen körs när programmet startar
int main()
{
	// Initierar klasser.
	// MessageBus hanterar skickandet av meddelande mellan moduler
	MessageBus bus;
	// UI hanterar användargränssnittet
	UI ui(bus);
	// Klass som representerar fönstret; kommer från det externa biblioteket GLFW, vilket hanterar fönstret
	GLFWwindow* window;
	// Klass som hanterar rendering
	Renderer renderer;
	// Initiera renderaren, vilket också startar ett GLFW-fönster
	renderer.init(windowWidth, windowHeight, "Testing Geemu", window);

	// Klass som representerar den bana som spelet spelas på. Genererar just nu en testbana som är 100x100 rutor stor
	Map map;
	map.generate(100, 100);

	// En klass som hanterar ett par av OpenGL-shaders.
	Shader ourShader("vertexShader.vert", "fragmentShader.frag");

	// Skapar spelaren. Detta ska senare vara implementerat i en klass
	int width, height, nrChannels;
	// Använder biblioteket stbi för att ladda in en PNG-bild i minnet. "stbi"-namespacet är mitt eget, då stbi är ett c-bibliotek och därmed har globala funktioner
	unsigned char* data = stbi::loadImage("player.png", width, height, nrChannels);
	// Funktionen returnerar en pekare till början på bilddatan, eller -1 ifall den misslyckades. I så fall, avsluta programmet.
	// I framtiden ska jag troligtvis implementera felmeddelanden för detta.
	if (!data)
	{
		return -1;
	}
	// Generera ett unikt ID som behövs för OpenGL:s texturer
	unsigned int playerTexture{ utils::generateTextureId() };
	// Koden nedan skapar en textur och läser in bilddatan till den texturen
	glGenTextures(1, &playerTexture);
	glBindTexture(GL_TEXTURE_2D, playerTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Frigör bilddatan från minnet då den ej behövs längre
	stbi::freeImage(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Skapar en ny spelare, och assignar den till min MessageBus. Detta ska då också abstraheras bort.
	Player* p = new Player(bus);
	p->texture = playerTexture;
	p->posX = 1;
	p->posY = 1;
	// Lägger till spelaren till Drawables, som kommer att ritas på banan
	drawables.push_back(p);

	// Lägger till en callback i MessageBusen för zoomning.
	bus.registerEventCallback(&zoomCallback);

	// Då jag just nu sparar en kopia i namespacet InputManager måste denna assignering ske efter att bussen har modifierats klart.
	InputManager::bus = bus;
	// Sätter några callbacks som ges av GLFW till funktioner i InputManager
  	glfwSetKeyCallback(window, &InputManager::keyCallback);
	glfwSetCharCallback(window, InputManager::textCallback);
	// Ställer in default-kontroller på InputManager. Utan detta skulle inga kontroller fungera.
	InputManager::setDefaultBindings();

	// glfwWindwShouldClose(GFLWwindow*) är en funktion som kollar ifall användaren har begärt att fönstret ska stängas ner, t ex genom att
	// klicka på krysset uppe till höger på fönstret. Detta kan också begäras av programmet själv.
	while (!glfwWindowShouldClose(window))
	{		 
		// Gör så att kameran följer efter spelaren
		camera.posX = -p->posX;
		camera.posY = -p->posY;

		// Renderar allting
		renderer.render(map, drawables, camera, ourShader, window);

		// GLFW-funktion som hämtar användar-input och anropar respektive callback
		glfwPollEvents();
	}
	// Avslutar GLFW
	glfwTerminate();
	// Frigör alla Drawables i drawables
	for (Drawable *d : drawables)
	{
		delete d;
	}
	// Avslutar programmet.
	return 0;
}