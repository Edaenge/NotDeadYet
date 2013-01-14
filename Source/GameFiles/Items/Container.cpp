#include "Container.h"


Container::Container(const unsigned int id, const unsigned int weight, const std::string& name, 
					 const unsigned int itemType, const std::string& itemDescription, 
					 const int maxUse, const int currUse) : Item(id, weight, name, itemType, itemDescription)
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
