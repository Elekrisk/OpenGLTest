#pragma once
#include "Inventory.h"
#include "MessageBus.h"

// Basen till alla varelser i spelet, inklusive spelaren
class Creature : public Drawable
{
	// Olika statusar. Anv�nds inte �n.
	int m_hp;
	int m_maxHp;
	int m_sp;
	int m_maxSp;

	int m_str;
	int m_dex;
	int m_int;
};

// Klass f�r monster.
class Monster : public Creature
{
	// De vapen etc. som monstret b�r p�
	Inventory equipment;
};

// Spelaren
class Player : public Creature
{
	// Spelarens inventory
	Inventory inventory;
public:
	// Constructor som anger vilken MessageBus som spelaren ska lyssna p� efter events
	Player(MessageBus &bus);
	// Den callback som blir anropad av GLFW
	bool eventCallback(MessageBus::EventType event, const std::string &info);
};