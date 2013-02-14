

#include "Bandage.h"

Bandage::Bandage() : Item()
{
	this->zStacking = true;
}

Bandage::Bandage(const unsigned int ID, const unsigned int Type, const unsigned int SubType)
	: Item(ID, Type, SubType)
{
	this->zStacking = true;
}

Bandage::Bandage( const unsigned int Type, const unsigned int SubType )
	: Item(Type, SubType)
{
	this->zStacking = true;
}

Bandage::~Bandage()
{

}

Bandage::Bandage(const Bandage& other)
{
	this->zID = other.zID;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
}

Bandage::Bandage(const Bandage* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zItemSubType = other->zItemSubType;
	this->zItemDescription = other->zItemDescription;
}

bool Bandage::Use()
{
	if(this->zStacks > 0)
	{
		this->zStacks--;
		return true;
	}
	return false;
}

std::string Bandage::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);

	return msg;
}