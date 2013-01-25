#include "Container.h"


Container::Container(const long id, const unsigned int itemType, 
					 const int maxUse, const int currUse) : Item(id, itemType)
{
	this->zStacking = false;
	this->zMaxUses = maxUse;
	this->zUsesRemaining = currUse;
}

Container::Container(const Container& other)
{
	this->zID = other.zID;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zMaxUses = other.zMaxUses;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zUsesRemaining = other.zUsesRemaining;
	this->zItemDescription = other.zItemDescription;
}

Container::Container(const Container* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zMaxUses = other->zMaxUses;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zUsesRemaining = other->zUsesRemaining;
	this->zItemDescription = other->zItemDescription;
}

Container::~Container()
{

}

bool Container::Use()
{
	if (this->zUsesRemaining > 0)
	{
		this->zUsesRemaining -= 1;
		return true;
	}
	return false;
}
