#include "GameFiles/Items/Container.h"


Container::Container(const unsigned int id, const unsigned int weight, const std::string& name, 
					 const unsigned int itemType, const std::string& itemDescription, 
					 const float maxSize, const float currSize) : Item(id, weight, name, itemType, itemDescription)
{
	this->zMaxSize = maxSize;
	this->zCurrentSize = currSize;
}

Container::~Container()
{

}

void Container::Use()
{

}
