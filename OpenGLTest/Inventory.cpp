#include "Inventory.h"

std::vector<ItemStack> Inventory::getItems() const
{
	// Returnerar den array som innehåller alla ItemStacks.
	return m_items;
}