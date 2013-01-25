
#include "Gear.h"

Gear::Gear(const long ID, const unsigned int TYPE) 
		   : Item(ID, TYPE)
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
	this->zItemDescription = other->zItemDescription;
}

Gear::~Gear()
{

}

bool Gear::Use()
{
	return true;
}