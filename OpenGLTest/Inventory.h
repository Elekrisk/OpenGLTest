#pragma once
#include "Item.h"
#include <vector>

// En Inventory som innehåller 'stacks' med Items. Avänds ej än.
class Inventory
{
	std::vector<ItemStack> m_items;
public:
	std::vector<ItemStack> getItems() const;
	bool removeItem(ItemStack, int amount);
	bool addItem();
};