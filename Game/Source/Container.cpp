#include "Container.h"

Container::Container() : Item()
{
	this->zStacking = false;
	this->zMaxUses = 0;
	this->zUsesRemaining = 0;
}

Container::Container(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType, 
					 const int maxUse, const int currUse) : Item(ID, itemType, itemSubType)
{
	this->zStacking = false;
	this->zMaxUses = maxUse;
	this->zUsesRemaining = currUse;
}

Container::Container(const unsigned int itemType, const unsigned int itemSubType,
					 const int maxUse, const int currUse) : Item(itemType, itemSubType)
{
	this->zStacking = false;
	this->zMaxUses = maxUse;
	this->zUsesRemaining = currUse;
}

Container::~Container()
{

}

Container::Container(const Container& other)
{
	if (other.zID == 0)
		this->GenerateID();
	else
		this->zID = other.zID;

	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zMaxUses = other.zMaxUses;
	this->zStacking = other.zStacking;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zSlotSize = other.zSlotSize;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zUsesRemaining = other.zUsesRemaining;
	this->zItemDescription = other.zItemDescription;
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

std::string Container::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);

	msg += NMC->Convert(MESSAGE_TYPE_CONTAINER_MAX, (float)this->zMaxUses);
	msg += NMC->Convert(MESSAGE_TYPE_CONTAINER_CURRENT, (float)this->zUsesRemaining);
	
	return msg;
}