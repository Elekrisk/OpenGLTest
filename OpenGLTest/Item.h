#pragma once
#include "Drawable.h"

// En klass för alla saker som en spelare kan ta upp. Ej implementerad
class Item
{

};

// Ett sätt att ordna Items. Innehåller vilken typ av Item och hur många. Dessa kan inte innehålla Items som måste ha olika parametrar, som
// t ex svärd av samma typ men med olika namn.
class ItemStack
{
	Item m_item;
	int m_amount;
public:
	Item getItem() const;
	int getAmount() const;
};

// En klass för Items som ligger på banan.
class ItemEntity : public Drawable
{
	ItemStack m_itemStack;
};