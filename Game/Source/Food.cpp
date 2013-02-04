#include "Food.h"

Food::Food(const long id, const unsigned int TYPE, const float hunger)
	: Item(id, TYPE)
{
	this->zHunger = hunger;
	this->zStacking = true;
}

Food::Food(const Food& other)
{
	this->zID = other.zID;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zHunger = other.zHunger;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zItemDescription = other.zItemDescription;
}

Food::Food(const Food* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zHunger = other->zHunger;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zItemDescription = other->zItemDescription;
}

Food::~Food()
{

}

bool Food::Use()
{
	if (this->zStacks > 0)
	{
		this->zStacks--;
		return true;
	}
	return false;
}

std::string Food::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);

	msg += NMC->Convert(MESSAGE_TYPE_HUNGER, (float)this->zHunger);

	return msg;
}
