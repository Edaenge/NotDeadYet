#include "GameFiles/Items/Container.h"


Container::Container(const unsigned int id, const unsigned int weight, const std::string& name, 
					 const unsigned int itemType, const std::string& itemDescription, 
					 const int maxUse, const int currUse) : Item(id, weight, name, itemType, itemDescription)
{
	this->zMaxUses = maxUse;
	this->zUsesRemaining = currUse;
}

Container::~Container()
{

}

void Container::Use()
{
	if (this->zUsesRemaining > 0)
	{
		this->zUsesRemaining -= 1;
	}
}
