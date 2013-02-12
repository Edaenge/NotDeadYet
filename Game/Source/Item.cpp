#include "Item.h"

static unsigned int NextITemID = 0;

Item::Item() : 
	zID(NextITemID++)
{
	this->zStacks = 1;
	this->zWeight = 0;
	this->zItemName = "none";
	this->zIconPath = "none";
	this->zItemType = 0;
	this->zItemDescription = "none";
	this->zMeshModel = "none";
}

Item::Item(const unsigned int itemType, const unsigned int itemSubType) : 
	zID(NextITemID++)
{
	this->zStacks = 1;
	this->zStacks = 1;
	this->zWeight = 0;
	this->zItemType = itemType;
	this->zItemName = "none";
	this->zIconPath = "none";
	this->zItemSubType = itemSubType;
	this->zItemDescription = "none";

}
std::string Item::ToMessageString(NetworkMessageConverter* NMC)
{
	std::string msg;

	msg = NMC->Convert(MESSAGE_TYPE_ITEM_ID, (float)this->zID);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_TYPE, (float)this->zItemType);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, this->zItemDescription);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_NAME, this->zItemName);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float) this->zWeight);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float) this->zStacks);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_ICON_PATH, this->zIconPath);

	return msg;
}