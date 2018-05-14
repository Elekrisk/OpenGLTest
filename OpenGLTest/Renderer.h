#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include "Map.h"
#include "Camera.h"
#include "shaderclass.h"
#include "ui.h"

class Renderer
{
	// Storleken p� f�nstret. Vet faktiskt inte varf�r detta sparas separat fr�n de globala variablerna i Main.cpp
	int windowWidth{ 0 };
	int windowHeight{ 0 };

	// Den VAO som anv�nds av alla objekt p� banan. UI kommer ej anv�nda denna.
	unsigned int defaultVAO{ 0 };

	// Klasser som ges av FreeType och som hanterar skrivandet av glyfer till bilder.
	FT_Library library;
	FT_Face face;

	// Funktioner f�r att initiera respektive del av renderaren.
	void initGLFW();
	int initGLAD();
	int initFreeType();
	int initUI(); // Ej implementerad �n

	// Genererar den VAO som beskrivs ovan.
	void generateDefaultVAO();

	// Renderar olika delar av spelet.
	void renderFloor(const Map &map, const Camera &camera, Shader &shader);
	void renderDrawables(const std::vector<Drawable*> drawables, Camera &camera, Shader &shader);
	void renderUI(UI ui); // Av�nds, men g�r ingenting just nu
public:
	// Initierar renderaren
	int init(int windowWidth, int windowHeight, const std::string &windowTitle, GLFWwindow* &outWindow);
	// Skapar GLFW-f�nstret
	GLFWwindow* createWindow(int width, int height, const std::string &title);
	// Renderar sk�rmen
	void render(const Map &map, const std::vector<Drawable*> drawables, Camera &camera, Shader &shader, GLFWwindow* window);
	// Funktion som omvandlar en bild till en textur
	void assignTexture(Image &img, unsigned int texture);
};