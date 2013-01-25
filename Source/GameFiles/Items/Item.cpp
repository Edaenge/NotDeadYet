#include "Item.h"

Item::Item(const long id, const unsigned int itemType)
{
	this->zStacks = 1;
	this->InitValues(id, itemType);
}

Item::Item()
{
	this->zID = -1;
	this->zStacks = 1;
	this->zWeight = 0;
	this->zItemName = "none";
	this->zIconPath = "none";
	this->zItemType = 0;
	this->zItemDescription = "none";
}

void Item::InitValues(const int id, const unsigned int itemType, const unsigned int weight, 
					  const std::string& name, const std::string& description, const std::string iconPath)
{
	this->zID = id;
	this->zStacks = 1;
	this->zWeight = weight;
	this->zItemName = name;
	this->zIconPath = iconPath;
	this->zItemType = itemType;
	this->zItemDescription = description;
}