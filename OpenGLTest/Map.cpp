#include "Map.h"
#include <string>
#include <random>
#include "utils.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "stbi_stuff.h"
#include "ImageConstructor.h"

// Genererar banan
void Map::generate(std::mt19937 *generator, int width, int height)
{
	// Den textur som ska renderas.
	m_texture = utils::generateTextureId();

	// Storleken på banan.
	m_width = width;
	m_height = height;

	// Ändra storleken på vectorn som innehåller banan.
	m_map.resize(width * height);

	for (int i{ 0 }; i < width; ++i)
	{
		for (int j{ 0 }; j < height; ++j)
		{
			m_map.at(utils::map2DTo1D(i, j, width)) = utils::getRandomInt(generator, 0, 10) > 1 ? 0 : 1;
		}
	}
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	int width1, height1, nrChannels1, width2, height2, nrChannels2;
	unsigned char* data1 = stbi::loadImage("brick.png", width1, height1, nrChannels1);
	unsigned char* data2 = stbi::loadImage("floor.png", width2, height2, nrChannels2);
	if (data1 && data2)
	{
		Image brickImg(data1, width1, height1, nrChannels1);
		Image floorImg(data2, width2, height2, nrChannels2);

		std::vector<Image> imgs{ floorImg, brickImg };
		Image mapImg = ImageConstructor::constructTiled(getMap(), getWidth(), imgs);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mapImg.getWidth(), mapImg.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mapImg.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi::freeImage(data1);
	stbi::freeImage(data2);
}

void Map::generate(int width, int height)
{
	std::random_device rd{};
	std::mt19937 generator{};

	generate(&generator, width, height);
}

void Map::generate(unsigned int seed, int width, int height)
{
	std::random_device rd{ };
	std::mt19937 generator{ };
	generator.seed(seed);

	generate(&generator, width, height);
}

std::vector<int> Map::getMap() const
{
	return m_map;
}

int Map::getWidth() const
{
	return m_width;
}

int Map::getHeight() const
{
	return m_height;
}

unsigned int Map::getTexture() const
{
	return m_texture;
}



void Floor::Generate(std::string seed)
{
	
}