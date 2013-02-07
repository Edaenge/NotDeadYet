#include "Gear.h"

Gear::Gear( const unsigned int ID, const unsigned int Type, const unsigned int SubType )
	: Item(ID, Type, SubType)
{
	this->zStacking = false;
}
Gear::Gear(const Gear& other)
{
	this->zID = other.zID;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
}

Gear::Gear(const Gear* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zItemSubType = other->zItemSubType;
	this->zItemDescription = other->zItemDescription;
}

Gear::~Gear()
{

}

bool Gear::Use()
{
	return true;
}

std::string Gear::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);

	return msg;
}
