#include "Food.h"

Food::Food() : Item()
{
	this->zHunger = 0;
	this->zStacking = true;
}

Food::Food(const unsigned int ID, const unsigned int Type, const unsigned int SubType, const float hunger)
	: Item(ID, Type, SubType)
{
	this->zHunger = hunger;
	this->zStacking = true;
}

Food::Food(const unsigned int Type, const unsigned int SubType, const float hunger)
	: Item(Type, SubType)
{
	this->zHunger = hunger;
	this->zStacking = true;
}

Food::~Food()
{

}

Food::Food(const Food& other)
{
	if (other.zID == 0)
		this->GenerateID();
	else
		this->zID = other.zID;

	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zHunger = other.zHunger;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zStacking = other.zStacking;
	this->zSlotSize = other.zSlotSize;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
}

bool Food::Use()
{
	if (this->zStacks > 0)
	{
		this->zStacks--;
		return true;
	}
	return false;
}

std::string Food::ToMessageString(NetworkMessageConverter* NMC)
{
	std::string msg = Item::ToMessageString(NMC);

	msg += NMC->Convert(MESSAGE_TYPE_HUNGER, (float)this->zHunger);

	return msg;
}
