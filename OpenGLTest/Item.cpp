#include "Item.h"

Item ItemStack::getItem() const
{
	return m_item;
}

int ItemStack::getAmount() const
{
	return m_amount;
}