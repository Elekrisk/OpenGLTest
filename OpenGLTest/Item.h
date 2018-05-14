#pragma once
#include "Drawable.h"

// En klass f�r alla saker som en spelare kan ta upp. Ej implementerad
class Item
{

};

// Ett s�tt att ordna Items. Inneh�ller vilken typ av Item och hur m�nga. Dessa kan inte inneh�lla Items som m�ste ha olika parametrar, som
// t ex sv�rd av samma typ men med olika namn.
class ItemStack
{
	Item m_item;
	int m_amount;
public:
	Item getItem() const;
	int getAmount() const;
};

// En klass f�r Items som ligger p� banan.
class ItemEntity : public Drawable
{
	ItemStack m_itemStack;
};