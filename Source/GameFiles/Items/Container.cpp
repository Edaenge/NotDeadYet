#include "Container.h"


Container::Container(const long id, const unsigned int itemType, 
					 const int maxUse, const int currUse) : Item(id, itemType)
{
	this->zStacking = false;
	this->zMaxUses = maxUse;
	this->zUsesRemaining = currUse;
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
