#pragma once
#include "Item.h"
#include <vector>

// En Inventory som inneh�ller 'stacks' med Items. Av�nds ej �n.
class Inventory
{
	std::vector<ItemStack> m_items;
public:
	std::vector<ItemStack> getItems() const;
	bool removeItem(ItemStack, int amount);
	bool addItem();
};