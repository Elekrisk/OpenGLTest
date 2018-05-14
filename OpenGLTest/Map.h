#pragma once
#include <vector>
#include "Creature.h"
#include <string>
#include <random>

class Floor;

// Klass som inneh�ller information om banan. En bana har flera v�ningar (d� spelet just nu endast har en v�ning kallas �ven
// v�ningen helt enkelt f�r'banan'). Just nu �r Floor och Map ihopslagen till en enda klass, men dessa ska senare separeras.
class Map
{
	std::vector<Floor> m_floors;
	std::vector<int> m_map;

	int m_width{ 0 };
	int m_height{ 0 };

	unsigned int m_texture;

	// Genererar banan med den specifierade generatorn och storleken.
	void generate(std::mt19937 *generator, int width, int height);
public:
	// Genererar banan med en default-generator
	void generate(int width, int height);
	// Samma som ovan, men med ett seed
	void generate(unsigned int seed, int width, int height);
	std::vector<int> getMap() const;

	int getWidth() const;
	int getHeight() const;

	unsigned int getTexture() const;
};

// En klass f�r en v�ning. Anv�nds just nu inte.
class Floor
{
	std::vector<int> map;
	std::vector<Monster> m_monsters;

public:
	void Generate(std::string seed);
};