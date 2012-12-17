#include "GameFiles/Items/Item.h"

Item::Item( const int id, const unsigned int itemType )
{
	this->InitValues(id, itemType);
}

Item::Item(const unsigned int id, const unsigned int weight, const std::string& name, const unsigned int itemType)
{
	this->InitValues(id, itemType, weight, name);
}

void Item::InitValues(const unsigned int id, const unsigned int itemType, const unsigned int weight, const std::string& name)
{
	this->zID = id;
	this->zWeight = weight;
	this->zItemName = name;
	this->zItemType = itemType;
}
