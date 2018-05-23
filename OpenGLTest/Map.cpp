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
			// Skriver antingen 1 eller 0 till vectorn, beroende på vad för tal funktionen getRandomInt() ger. 0 representerar golv och 1 en vägg.
			m_map.at(utils::map2DTo1D(i, j, width)) = utils::getRandomInt(generator, 0, 10) > 1 ? 0 : 1;
		}
	}
	// Genererar det texture som banan ska använda, och binder det till GL_TEXTURE_2D för fortsatt bearbetning.
	glGenTextures(utils::generateTextureId(), &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Sätter upp variabler och laddar in datan från de två bilderna brick.png och floor.png
	int width1, height1, nrChannels1, width2, height2, nrChannels2;
	unsigned char* data1 = stbi::loadImage("brick.png", width1, height1, nrChannels1);
	unsigned char* data2 = stbi::loadImage("floor.png", width2, height2, nrChannels2);
	// Kollar ifall dataläsningen fungerade
	if (data1 && data2)
	{
		// Skapar Image-instanser för de två bilderna
		Image brickImg(data1, width1, height1, nrChannels1);
		Image floorImg(data2, width2, height2, nrChannels2);

		// En vector som används av cunstructTiled(). Numren som finns i vectorn m_map används som index för att hämta bilder ur denna vector. 
		std::vector<Image> imgs{ floorImg, brickImg };
		// constructTiled() använder vectorerna m_map (banan) och imgs(bilderna) och sätter ihop bilderna till en enda stor bild med m_map som guide till
		// vilka bilder som ska vara var.
		Image mapImg = ImageConstructor::constructTiled(getMap(), getWidth(), imgs);

		// Läser in mapImg till texturen som genererades ovan
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mapImg.getWidth(), mapImg.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mapImg.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		// ifall dataläsningen misslyckades, skriv ett felmeddelande.
		std::cout << "Failed to load texture\n";
	}
	// Ställer in parametrar hur bilden ska skalas, och frigör sedan de två bildernas data.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi::freeImage(data1);
	stbi::freeImage(data2);
}

void Map::generate(int width, int height)
{
	// Ifall ingen generator ges, skapa en ny generator och använd den.
	// Detta är egentligen inget bra sätt att göra detta, då jag tror att generatorn använder tiden som seed.
	std::random_device rd{};
	std::mt19937 generator{};

	generate(&generator, width, height);
}

void Map::generate(unsigned int seed, int width, int height)
{
	// Samma som ovan, men avänder ett bestämt seed
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


// Används ej än
void Floor::Generate(std::string seed)
{
	// TBD
}