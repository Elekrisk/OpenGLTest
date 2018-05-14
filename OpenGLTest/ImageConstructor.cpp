#include "ImageConstructor.h"
#include "utils.h"
#include <iostream>

Image ImageConstructor::constructTiled(std::vector<int> map, int mapWidth, std::vector<Image> images)
{
	// Skriver ut olika v�rden av debug-anledningar.
	// D� endast mapWidth ges som parameter r�nkas mapHeight ut fr�n vectorn och bildstorlekarna tas fr�n den f�rsta bilden i images-vectorn.
	// Funktionen antar att alla bilder har samma storlek.
	std::cout << mapWidth << std::endl;
	int mapHeight = map.size() / mapWidth;
	std::cout << mapHeight << std::endl;
	int imageWidth = images[0].getWidth();
	std::cout << imageWidth << std::endl;
	int imageHeight = images[0].getHeight();
	std::cout << imageHeight << std::endl;
	int channels = images[0].getChannels();
	// Skriver ut antalet bytes som den f�rdiga bilden kommer att ta upp.
	std::cout << mapWidth * imageWidth * mapHeight * imageHeight * channels << std::endl;

	// Arrayen som kommer att inneh�lla bildens data.
	unsigned char* data = new unsigned char[mapWidth * imageWidth * mapHeight * imageHeight * channels];
	for (int i{ 0 }; i < mapWidth; ++i)
	{
		for (int j{ 0 }; j < mapHeight; ++j)
		{
			// Funktionen sparar den bild som ska vara p� koordinaterna (i, j) i img f�r enklare �tkomst
			Image img = images[map[utils::map2DTo1D(i, j, mapWidth)]];
			for (int k{ 0 }; k < imageWidth; ++k)
			{
				for (int l{ 0 }; l < imageHeight; ++l)
				{
					// I denna loop best�mmer (k, l) en pixel i den bild som ska kopieras in i den stora bilden.
					for (int m{ 0 }; m < channels; ++m)
					{
						// Varje byte kopieras var f�r sig, vilket g�r att denna funktion fungerar med olika antal kanaler

						// Kopierar in en byte fr�n den lilla bilden till den stora bilden p� r�tt plats.
						data[utils::map2DTo1D(i * imageWidth + k, j * imageHeight + l, mapWidth * imageWidth) * channels + m] = getDataUnit(&img, k, l, m);
					}
				}
			}
		}
	}
	// Skapar en Image av datan.
	Image mapImage(data, mapWidth * imageWidth, mapHeight * imageHeight, channels);
	// Returnerar bilden.
	return mapImage;
}

// Kopierar in en bild i en annan bild
void ImageConstructor::composite(Image *src, Image *dest, int x, int y)
{
	// Ifall k�llan och destinationen ej har samma antal kanaler, avbryt funktionen.
	if (src->getChannels() != dest->getChannels())
	{
		return;
	}
	// Avbryt �ven om k�llbilden skulle hamna utanf�r destinationsbilden
	if (x + src->getWidth() > dest->getWidth() || y + src->getHeight() > dest->getHeight() || x < 0 || y < 0)
	{
		return;
	}

	for (int i{ x }; i < src->getWidth(); ++i)
	{
		for (int j{ y }; j < src->getHeight(); ++j)
		{
			for (int k{ 0 }; k < src->getChannels(); ++k)
			{
				// Skriv �ver data i destinationsbilden med data fr�n k�llbilded
				dest->getData()[utils::map2DTo1D(i, j, dest->getWidth()) * dest->getChannels() + k] = getDataUnit(src, i - x, j - x, k);
			}
		}
	}
}

// Kopiera in en bild i en annan bild med en gr�skalsmask.
void ImageConstructor::composite(Image *src, Image *dest, Image *mask, int x, int y)
{
	// Avbryt funktionen ifall k�llbilden skulle hamna delvis utanf�r destinationsbilden.
	if (x + src->getWidth() > dest->getWidth() || y + src->getHeight() > dest->getHeight() || x < 0 || y < 0)
	{
		return;
	}
	// Avbryt �ven ifall k�llan och masken ej har samma storlek.
	if (src->getHeight() != mask->getHeight() || src->getWidth() != mask->getWidth())
	{
		return;
	}

	for (int i{ x }; i < src->getWidth(); ++i)
	{
		for (int j{ y }; j < src->getHeight(); ++j)
		{
			for (int k{ 0 }; k < src->getChannels(); ++k)
			{
				// Samma som i funktionen ovan, men anv�nd en lerp-funktion f�r att blanda bilddatan beroende p� v�rdet p� masken
				dest->getData()[utils::map2DTo1D(i, j, dest->getWidth()) * dest->getChannels() + k] = utils::lerp(getDataUnit(mask, i - x, j - y, 0) / 255.0f, getDataUnit(dest, x, y, k), getDataUnit(src, i - x, j - x, k));
			}
		}
	}
}

// Skriv �ver bilden med en helf�rg, med en mask
void ImageConstructor::composite(Image *dest, Image *mask, Color color, int x, int y)
{
	// Avbryt funktionen ifall k�llbilden hamnar utanf�r destinationsbilden.
	if (x + mask->getWidth() > dest->getWidth() || y + mask->getHeight() > dest->getHeight() || x < 0 || y < 0)
	{
		return;
	}

	for (int i{ x }; i < mask->getWidth(); ++i)
	{
		for (int j{ y }; j < mask->getHeight(); ++j)
		{
			for (int k{ 0 }; k < dest->getChannels(); ++k)
			{
				// Samma som funktionen ovan, men blandar mellan f�rgen och destinationsbilden.
				dest->getData()[utils::map2DTo1D(i, j, dest->getWidth()) * dest->getChannels() + k] = utils::lerp(getDataUnit(mask, i - x, j - y, 0) / 255.0f, getDataUnit(dest, x, y, k), color.m_channels[k]);
			}
		}
	}
}


unsigned char ImageConstructor::getDataUnit(Image *img, int x, int y, int channel)
{
	// H�mta datan fr�n bilden, extrahera r�tt byte och returnera den.
	unsigned char* data = img->getData();
	int index{ utils::map2DTo1D(x, y, img->getWidth()) * img->getChannels() + channel };
	return data[index];
}



Color::Color(const std::vector<unsigned char> &colors)
{
	// Assigna alla kanaler till f�rgen som anges
	m_channels.resize(colors.size());
	for (int i{ 0 }; i < m_channels.size(); ++i)
	{
		m_channels[i] = colors[i];
	}
}

Image::Image()
{
	m_data = nullptr;
	m_width = 0;
	m_height = 0;
	m_channels = 0;
}

Image::Image(unsigned char* data, int width, int height, int channels)
{
	m_data = data;
	m_width = width;
	m_height = height;
	m_channels = channels;
}

unsigned char* Image::getData()
{
	return m_data;
}

void Image::setData(unsigned char* newData)
{
	m_data = newData;
}

int Image::getWidth() const
{
	return m_width;
}

int Image::getHeight() const
{
	return m_height;
}

int Image::getChannels() const
{
	return m_channels;
}