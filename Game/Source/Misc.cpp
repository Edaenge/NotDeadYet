#include "Misc.h"

Misc::Misc()
{
	this->zStacking = true;
}

Misc::Misc(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType)
	: Item(ID, itemType, itemSubType)
{
	this->zStacking = true;
}

Misc::Misc(const unsigned int itemType, const unsigned int itemSubType)
	: Item(itemType, itemSubType)
{
	this->zStacking = true;
}

Misc::Misc(const Misc& other )
{
	if (other.zID == 0)
		this->GenerateID();
	else
		this->zID = other.zID;

	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zStacking = other.zStacking;
	this->zSlotSize = other.zSlotSize;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
}

Misc::~Misc()
{

}

bool Misc::Use()
{
	if (this->zStacks > 0)
	{
		this->zStacks--;
		return true;
	}
	return false;
}