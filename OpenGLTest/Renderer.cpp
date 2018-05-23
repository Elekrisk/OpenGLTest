#include "Renderer.h"
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Camera.h"
#include "shaderclass.h"
#include <functional>
#include "Main.h"
#include "ui.h";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void Renderer::initGLFW()
{
	// Initierar GLFW
	glfwInit();
	// St�ller in GLFW-parametrar, d.v.s OpenGL-version och -profil.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int Renderer::initGLAD()
{
	// Initierar GLAD och kollar ifall initieringen misslyckades.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Skriver ut ett felmeddelande och avbryter programmet.
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 0;
}

int Renderer::initFreeType()
{
	// Initierar FreeType, ett bibliotek f�r att generera glyfer
	FT_Error error = FT_Init_FreeType(&library);
	// Felhantering
	if (error)
	{
		std::cout << "Failed to initialize FreeType" << std::endl;
		return -1;
	}
	// L�ser in arial-typsnittet, d� det f�ljer med Windows
	char fontPath[]{ "C:\\Windows\\Fonts\\arial.ttf" };
	error = FT_New_Face(library, fontPath, 0, &face);
	// Felhantering
	if (error == FT_Err_Unknown_File_Format)
	{
		std::cout << "FreeType could not read font file type" << std::endl;
		return -1;
	}
	else if (error)
	{
		std::cout << "FreeType failed to load font at " << fontPath << std::endl;
		return -1;
	}
	// St�ller in storleken p� typsnittet till 16px
	error = FT_Set_Pixel_Sizes(face, 0, 16);
	// Felhantering
	if (error)
	{
		std::cout << "FreeType failed to set the size of face " << '0' << std::endl;
	}
}

GLFWwindow* Renderer::createWindow(int width, int height, const std::string &title)
{
	// Anv�nder GLFWs funktion
	return glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void Renderer::generateDefaultVAO()
{
	// Vertexerna i VBO:n (Vertex Buffer Object). De f�rsta tre float-v�rderna i raderna �r x-, y- och z-positioner, medan de tv� sista p� raden
	// �r texturkoordinater, dvs hur texturen ska "mappas" till figuren.
	// Det �r fyra vertexer, en f�r varje rad, som bildar en kvadrat.
	float vertices[]
	{
		0.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f
	};
	// OpenGL kan inte rita ut fyrh�rningar direkt, utan m�ste anv�nda sig av trianglar. F�r att undvika att beh�va skriva ut 6 vertexer ist�llet f�r 4
	// kan man anv�nda ett EBO (Element Buffer Object) som s�ger till OpenGL i vilken ordning som vertexerna ska ritas.
	// Siffrorna 0 och 2 finns med tv� g�nger, vilket g�r att de kommer att ritas ut tv� g�nger, en f�r varje triangel.
	// En triangel per rad.
	unsigned int indices[]
	{
		0, 1, 2,
		0, 2, 3
	};
	// Genererar ett VAO, vilket sparar referenser till VBO:n och EBO:n och shader-bindningar.
	glGenVertexArrays(1, &defaultVAO);
	glBindVertexArray(defaultVAO);

	// Generar ett VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Genererar ett EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// L�gger in vertexerna i VBO:n
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Binder v�rden i VBO:n till pekare
	// De tre f�rsta float-v�rderna binds till attributpekaren 0 och de sista tv� v�rderna till pekaren 1.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

int Renderer::init(int width, int height, const std::string &windowTitle, GLFWwindow* &outWindow)
{
	// St�ller in f�nstrets storlek
	windowWidth = width;
	windowHeight = height;
	// Initierar GLFW
	initGLFW();
	// Skapa f�nstret
	GLFWwindow* window = createWindow(windowWidth, windowHeight, windowTitle);
	// Felhantering
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		// Avslutar GLFW
		glfwTerminate();
		return -1;
	}
	// S�ger till GLFW att f�nstret �r det kontext som �r aktivt.
	glfwMakeContextCurrent(window);
	// S�tter outWindow till window
	outWindow = window;
	// Initierar GLAD + felhantering
	if (initGLAD())
	{
		return -1;
	}
	// S�tter den callback som ska anropas n�r f�nstrets storlek �ndras
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	// G�r s� att OpenGL sparar djupet p� det man ritar ut, s� att saker som ska hamna ovanp� andra saker ocks� g�r det.
	glEnable(GL_DEPTH);
	// Genererar en VAO som anv�nds f�r allt p� banan
	generateDefaultVAO();
	// Initierar FreeType
	initFreeType();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// S�ger till OpenGL att rita p� hela f�nstret, och uppdatera de globala variablerna.
	glViewport(0, 0, width, height);
	::windowWidth = width;
	::windowHeight = height;
};




void Renderer::render(const Map &map, const std::vector<Drawable*> drawables, Camera &camera, Shader &shader, GLFWwindow* window)
{
	// Sparar de globala variablerna till de lokala
	windowWidth = ::windowWidth;
	windowHeight = ::windowHeight;
	// rensar sk�rmen
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// S�ger till OpenGL att detta �r den shader som ska anv�ndas vid ritning
	shader.use();

	// Skapar en projektionsmatris som anv�nds f�r att manipulera objekten som ska ritas s� att allt som ses av den virtuella kameran hamnar mellan -1 och 1 p� alla axlar,
	// vilket �r det enda som ritas ut p� sk�rmen.
	glm::mat4 projection;
	projection = glm::ortho(-windowWidth / 2 / camera.m_zoom / 100, windowWidth / 2 / camera.m_zoom / 100, -windowHeight / 2 / camera.m_zoom / 100, windowHeight / 2 / camera.m_zoom / 100, 0.01f, 100.0f);
	camera.m_projection = projection;

	// Renderar banan och allt som finns p� den
	renderFloor(map, camera, shader);
	renderDrawables(drawables, camera, shader);

	// Byter buffrarna s� att allt som ritats visas p� sk�rmen.
	glfwSwapBuffers(window);
}

void Renderer::renderFloor(const Map &map, const Camera &camera, Shader &shader)
{
	// G�r en matris som manipulerar det objekt som representerar banan s� att den f�r r�tt storlek.
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(map.getWidth(), map.getHeight(), 1.0f));

	// Matris som flyttar objekt beroende p� datan i den virtuella kameran.
	// Det �r ej kameran som flyttas, utan allting annat.
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(camera.m_posX, camera.m_posY, -3.0f));

	// H�mtar positionen av uniform-variablar (de variabler som �r samma f�r alla pixlar)
	unsigned int modelLocation = glGetUniformLocation(shader, "model");
	unsigned int viewLocation = glGetUniformLocation(shader, "view");
	unsigned int projectionLocation = glGetUniformLocation(shader, "projection");

	// S�tter v�rden p� uniform-variablarna
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.m_projection));

	// S�ger att det �r banans texture som ska anv�ndas n�r banan ritas, och att det �r default-VAO:n som ska anv�ndas.
	glBindTexture(GL_TEXTURE_2D, map.getTexture());
	glBindVertexArray(defaultVAO);
	// Ritar ut banan.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::renderDrawables(const std::vector<Drawable*> drawables, Camera &camera, Shader &shader)
{
	// H�mtar positionen f�r en uniform-variabel, n�rmare best�mt model-variabeln.
	// Denna variabel inneh�ller en matris som s�ger hur modellen ska manipuleras fr�n model-space till world-space.
	unsigned int modelLocation = glGetUniformLocation(shader, "model");
	// Loopar igenom allt om ska ritas p� banan.
	for (Drawable* d : drawables)
	{
		// Skapar en matris beroende p� positionen p� instansen
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(d->m_posX, d->m_posY, 0.0f));
		// S�tt uniform-variabeln model till denna matris
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		// Bind instanses texture
		glBindTexture(GL_TEXTURE_2D, d->m_texture);
		// Rita ut objektet
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Att rita ut varje objekt f�r sig �r det enklaste s�ttet, men ocks� det alternativ som �r minst effektivt.
	}
}

// Anv�nds ej �n
void Renderer::renderUI(UI ui)
{
	//std::vector<UIGroup> *groups = ui.getElementGroups();

	//std::sort(groups->begin(), groups->end());

	/*for (int i{ 0 }; i < groups->size(); ++i)
	{
		
	}*/
}

// Anv�nds ej �n
void Renderer::assignTexture(Image &img, unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}