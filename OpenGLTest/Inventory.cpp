#include "Inventory.h"

std::vector<ItemStack> Inventory::getItems() const
{
	// Returnerar den array som inneh�ller alla ItemStacks.
	return m_items;
}