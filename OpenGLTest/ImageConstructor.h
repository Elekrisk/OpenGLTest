#pragma once
#include <vector>
#include "utils.h"

class Image;
class Color;

// Ett namespace som har och g�ra med att ska och modifiera bilder.
namespace ImageConstructor
{
	// Anv�nds f�r att skapa banans textur. Given tv� vectors f�r banan respektive bilderna skapar den en stor textur med r�tt bilder p� r�tt plats.
	Image constructTiled(std::vector<int> map, int mapWidth, std::vector<Image> images);
	// Funktioner f�r att l�gga ihop tv� bilder. Ska anv�ndas flitigt i UI:n.
	void composite(Image *src, Image *dest, int x, int y);
	void composite(Image *src, Image *dest, Image *mask, int x, int y);
	void composite(Image *dest, Image *mask, Color color, int x, int y);
	// Funktion f�r att f� ut en byte ur datan fr�n r�tt st�lle.
	unsigned char getDataUnit(Image *img, int x, int y, int channel);
	// Oimplementerad funktion f�r att skapa en text-mask, som sedan kan anv�ndas med en av composite-funktionerna.
	Image createTextMask(const std::string &text, int fontSize);
};

// Enkel klass som inneh�ller v�rden f�r en f�rg med ett flytande antal kanaler
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

// Klass f�r att f�renkla hanteringen av bilder. Datan sparas som en pekare till en char-array
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