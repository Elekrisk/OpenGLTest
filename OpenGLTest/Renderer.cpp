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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int Renderer::initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 0;
}

int Renderer::initFreeType()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		std::cout << "Failed to initialize FreeType" << std::endl;
		return -1;
	}
	char fontPath[]{ "C:\\Windows\\Fonts\\arial.ttf" };
	error = FT_New_Face(library, fontPath, 0, &face);
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
	error = FT_Set_Pixel_Sizes(face, 0, 16);
	if (error)
	{
		std::cout << "FreeType failed to set the size of face " << '0' << std::endl;
	}
}

GLFWwindow* Renderer::createWindow(int width, int height, const std::string &title)
{
	return glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void Renderer::generateDefaultVAO()
{
	float vertices[]
	{
		0.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,	0.0f, 0.0f
	};
	unsigned int indices[]
	{
		0, 1, 2,
		0, 2, 3
	};
	glGenVertexArrays(1, &defaultVAO);
	glBindVertexArray(defaultVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

int Renderer::init(int width, int height, const std::string &windowTitle, GLFWwindow* &outWindow)
{
	windowWidth = width;
	windowHeight = height;
	initGLFW();
	GLFWwindow* window = createWindow(windowWidth, windowHeight, windowTitle);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	outWindow = window;
	if (initGLAD())
	{
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glEnable(GL_DEPTH);
	generateDefaultVAO();
	initFreeType();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	::windowWidth = width;
	::windowHeight = height;
};




void Renderer::render(const Map &map, const std::vector<Drawable*> drawables, Camera &camera, Shader &shader, GLFWwindow* window)
{
	windowWidth = ::windowWidth;
	windowHeight = ::windowHeight;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.use();

	glm::mat4 projection;
	projection = glm::ortho(-windowWidth / 2 / camera.zoom / 100, windowWidth / 2 / camera.zoom / 100, -windowHeight / 2 / camera.zoom / 100, windowHeight / 2 / camera.zoom / 100, 0.01f, 100.0f);
	camera.projection = projection;

	renderFloor(map, camera, shader);
	renderDrawables(drawables, camera, shader);

	glfwSwapBuffers(window);
}

void Renderer::renderFloor(const Map &map, const Camera &camera, Shader &shader)
{
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(map.getWidth(), map.getHeight(), 1.0f));

	glm::mat4 view;
	view = glm::translate(view, glm::vec3(camera.posX, camera.posY, -3.0f));

	unsigned int modelLocation = glGetUniformLocation(shader, "model");
	unsigned int viewLocation = glGetUniformLocation(shader, "view");
	unsigned int projectionLocation = glGetUniformLocation(shader, "projection");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.projection));

	glBindTexture(GL_TEXTURE_2D, map.getTexture());
	glBindVertexArray(defaultVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::renderDrawables(const std::vector<Drawable*> drawables, Camera &camera, Shader &shader)
{
	unsigned int modelLocation = glGetUniformLocation(shader, "model");
	for (Drawable* d : drawables)
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(d->posX, d->posY, 0.0f));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, d->texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void Renderer::renderUI(UI ui)
{
	//std::vector<UIGroup> *groups = ui.getElementGroups();

	//std::sort(groups->begin(), groups->end());

	/*for (int i{ 0 }; i < groups->size(); ++i)
	{
		
	}*/
}

void Renderer::assignTexture(Image &img, unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}