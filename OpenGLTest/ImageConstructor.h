#pragma once
#include <vector>
#include "utils.h"

class Image;
class Color;

// Ett namespace som har och göra med att ska och modifiera bilder.
namespace ImageConstructor
{
	// Används för att skapa banans textur. Given två vectors för banan respektive bilderna skapar den en stor textur med rätt bilder på rätt plats.
	Image constructTiled(std::vector<int> map, int mapWidth, std::vector<Image> images);
	// Funktioner för att lägga ihop två bilder. Ska användas flitigt i UI:n.
	void composite(Image *src, Image *dest, int x, int y);
	void composite(Image *src, Image *dest, Image *mask, int x, int y);
	void composite(Image *dest, Image *mask, Color color, int x, int y);
	// Funktion för att få ut en byte ur datan från rätt ställe.
	unsigned char getDataUnit(Image *img, int x, int y, int channel);
	// Oimplementerad funktion för att skapa en text-mask, som sedan kan användas med en av composite-funktionerna.
	Image createTextMask(const std::string &text, int fontSize);
};

// Enkel klass som innehåller värden för en färg med ett flytande antal kanaler
class Color
{
public:
	std::vector<char> m_channels;
	enum Indexes
	{
		RED,
		GREEN,
		BLUE,
		ALPHA
	};
	Color(const std::vector<unsigned char> &colors);
};

// Klass för att förenkla hanteringen av bilder. Datan sparas som en pekare till en char-array
class Image
{
	unsigned char* m_data;
	int m_width;
	int m_height;
	int m_channels;
public:
	Image();
	Image(unsigned char* data, int width, int height, int channels);

	unsigned char* getData();
	void setData(unsigned char* newData);
	int getWidth() const;
	int getHeight() const;
	int getChannels() const;
};