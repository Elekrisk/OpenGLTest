#pragma once

// Namespace som hanterar stb_image-funktioner
namespace stbi
{
	// Laddar in en bild i minnet till en char-array
	unsigned char* loadImage(const char* filename, int &width, int &height, int &channels, int expectedChannels = 0);
	// Frig�r en bild-char-array fr�n minnet
	void freeImage(unsigned char* data);
}