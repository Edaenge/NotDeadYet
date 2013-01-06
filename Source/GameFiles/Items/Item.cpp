#include "GameFiles/Items/Item.h"

Item::Item( const int id, const unsigned int itemType )
{
	this->InitValues(id, itemType);
}

Item::Item(const unsigned int id, const unsigned int weight, const std::string& name, 
		   const unsigned int itemType, const std::string& itemDescription)
{
	this->InitValues(id, itemType, weight, name, itemDescription);
}

void Item::InitValues(const int id, const unsigned int itemType, const unsigned int weight, 
					  const std::string& name, const std::string& description)
{
	this->zID = id;
	this->zWeight = weight;
	this->zItemName = name;
	this->zItemType = itemType;
	this->zItemDescription = description;
}