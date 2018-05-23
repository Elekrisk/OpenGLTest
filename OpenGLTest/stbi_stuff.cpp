#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stbi_stuff.h"

// Två enkla wrappers för stbi, då detta bibliotek är lite konstigt och bara kan inkluderas direkt i en fil.
unsigned char* stbi::loadImage(const char* filename, int &width, int &height, int &channels, int expectedChannels)
{
	return stbi_load(filename, &width, &height, &channels, expectedChannels);
}

void stbi::freeImage(unsigned char* data)
{
	stbi_image_free(data);
}