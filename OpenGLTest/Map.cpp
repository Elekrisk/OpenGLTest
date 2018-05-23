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

	// Storleken p� banan.
	m_width = width;
	m_height = height;

	// �ndra storleken p� vectorn som inneh�ller banan.
	m_map.resize(width * height);

	for (int i{ 0 }; i < width; ++i)
	{
		for (int j{ 0 }; j < height; ++j)
		{
			// Skriver antingen 1 eller 0 till vectorn, beroende p� vad f�r tal funktionen getRandomInt() ger. 0 representerar golv och 1 en v�gg.
			m_map.at(utils::map2DTo1D(i, j, width)) = utils::getRandomInt(generator, 0, 10) > 1 ? 0 : 1;
		}
	}
	// Genererar det texture som banan ska anv�nda, och binder det till GL_TEXTURE_2D f�r fortsatt bearbetning.
	glGenTextures(utils::generateTextureId(), &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// S�tter upp variabler och laddar in datan fr�n de tv� bilderna brick.png och floor.png
	int width1, height1, nrChannels1, width2, height2, nrChannels2;
	unsigned char* data1 = stbi::loadImage("brick.png", width1, height1, nrChannels1);
	unsigned char* data2 = stbi::loadImage("floor.png", width2, height2, nrChannels2);
	// Kollar ifall datal�sningen fungerade
	if (data1 && data2)
	{
		// Skapar Image-instanser f�r de tv� bilderna
		Image brickImg(data1, width1, height1, nrChannels1);
		Image floorImg(data2, width2, height2, nrChannels2);

		// En vector som anv�nds av cunstructTiled(). Numren som finns i vectorn m_map anv�nds som index f�r att h�mta bilder ur denna vector. 
		std::vector<Image> imgs{ floorImg, brickImg };
		// constructTiled() anv�nder vectorerna m_map (banan) och imgs(bilderna) och s�tter ihop bilderna till en enda stor bild med m_map som guide till
		// vilka bilder som ska vara var.
		Image mapImg = ImageConstructor::constructTiled(getMap(), getWidth(), imgs);

		// L�ser in mapImg till texturen som genererades ovan
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mapImg.getWidth(), mapImg.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mapImg.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		// ifall datal�sningen misslyckades, skriv ett felmeddelande.
		std::cout << "Failed to load texture\n";
	}
	// St�ller in parametrar hur bilden ska skalas, och frig�r sedan de tv� bildernas data.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi::freeImage(data1);
	stbi::freeImage(data2);
}

void Map::generate(int width, int height)
{
	// Ifall ingen generator ges, skapa en ny generator och anv�nd den.
	// Detta �r egentligen inget bra s�tt att g�ra detta, d� jag tror att generatorn anv�nder tiden som seed.
	std::random_device rd{};
	std::mt19937 generator{};

	generate(&generator, width, height);
}

void Map::generate(unsigned int seed, int width, int height)
{
	// Samma som ovan, men av�nder ett best�mt seed
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


// Anv�nds ej �n
void Floor::Generate(std::string seed)
{
	// TBD
}