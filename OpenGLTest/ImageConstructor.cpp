#include "ImageConstructor.h"
#include "utils.h"
#include <iostream>

Image ImageConstructor::constructTiled(std::vector<int> map, int mapWidth, std::vector<Image> images)
{
	// Skriver ut olika värden av debug-anledningar.
	// Då endast mapWidth ges som parameter ränkas mapHeight ut från vectorn och bildstorlekarna tas från den första bilden i images-vectorn.
	// Funktionen antar att alla bilder har samma storlek.
	std::cout << mapWidth << std::endl;
	int mapHeight = map.size() / mapWidth;
	std::cout << mapHeight << std::endl;
	int imageWidth = images[0].getWidth();
	std::cout << imageWidth << std::endl;
	int imageHeight = images[0].getHeight();
	std::cout << imageHeight << std::endl;
	int channels = images[0].getChannels();
	// Skriver ut antalet bytes som den färdiga bilden kommer att ta upp.
	std::cout << mapWidth * imageWidth * mapHeight * imageHeight * channels << std::endl;

	// Arrayen som kommer att innehålla bildens data.
	unsigned char* data = new unsigned char[mapWidth * imageWidth * mapHeight * imageHeight * channels];
	for (int i{ 0 }; i < mapWidth; ++i)
	{
		for (int j{ 0 }; j < mapHeight; ++j)
		{
			// Funktionen sparar den bild som ska vara på koordinaterna (i, j) i img för enklare åtkomst
			Image img = images[map[utils::map2DTo1D(i, j, mapWidth)]];
			for (int k{ 0 }; k < imageWidth; ++k)
			{
				for (int l{ 0 }; l < imageHeight; ++l)
				{
					// I denna loop bestämmer (k, l) en pixel i den bild som ska kopieras in i den stora bilden.
					for (int m{ 0 }; m < channels; ++m)
					{
						// Varje byte kopieras var för sig, vilket gör att denna funktion fungerar med olika antal kanaler

						// Kopierar in en byte från den lilla bilden till den stora bilden på rätt plats.
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
	// Ifall källan och destinationen ej har samma antal kanaler, avbryt funktionen.
	if (src->getChannels() != dest->getChannels())
	{
		return;
	}
	// Avbryt även om källbilden skulle hamna utanför destinationsbilden
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
				// Skriv över data i destinationsbilden med data från källbilded
				dest->getData()[utils::map2DTo1D(i, j, dest->getWidth()) * dest->getChannels() + k] = getDataUnit(src, i - x, j - x, k);
			}
		}
	}
}

// Kopiera in en bild i en annan bild med en gråskalsmask.
void ImageConstructor::composite(Image *src, Image *dest, Image *mask, int x, int y)
{
	// Avbryt funktionen ifall källbilden skulle hamna delvis utanför destinationsbilden.
	if (x + src->getWidth() > dest->getWidth() || y + src->getHeight() > dest->getHeight() || x < 0 || y < 0)
	{
		return;
	}
	// Avbryt även ifall källan och masken ej har samma storlek.
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
				// Samma som i funktionen ovan, men använd en lerp-funktion för att blanda bilddatan beroende på värdet på masken
				dest->getData()[utils::map2DTo1D(i, j, dest->getWidth()) * dest->getChannels() + k] = utils::lerp(getDataUnit(mask, i - x, j - y, 0) / 255.0f, getDataUnit(dest, x, y, k), getDataUnit(src, i - x, j - x, k));
			}
		}
	}
}

// Skriv över bilden med en helfärg, med en mask
void ImageConstructor::composite(Image *dest, Image *mask, Color color, int x, int y)
{
	// Avbryt funktionen ifall källbilden hamnar utanför destinationsbilden.
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
				// Samma som funktionen ovan, men blandar mellan färgen och destinationsbilden.
				dest->getData()[utils::map2DTo1D(i, j, dest->getWidth()) * dest->getChannels() + k] = utils::lerp(getDataUnit(mask, i - x, j - y, 0) / 255.0f, getDataUnit(dest, x, y, k), color.m_channels[k]);
			}
		}
	}
}


unsigned char ImageConstructor::getDataUnit(Image *img, int x, int y, int channel)
{
	// Hämta datan från bilden, extrahera rätt byte och returnera den.
	unsigned char* data = img->getData();
	int index{ utils::map2DTo1D(x, y, img->getWidth()) * img->getChannels() + channel };
	return data[index];
}



Color::Color(const std::vector<unsigned char> &colors)
{
	// Assigna alla kanaler till färgen som anges
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