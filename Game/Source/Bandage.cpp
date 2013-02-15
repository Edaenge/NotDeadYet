

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
	if (other.zID == 0)
		this->GenerateID();
	else
		this->zID = other.zID;

	this->zID = other.zID;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
	this->zStacking = other.zStacking;
}

Bandage::Bandage(const Bandage* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zMeshModel = other->zMeshModel;
	this->zItemSubType = other->zItemSubType;
	this->zItemDescription = other->zItemDescription;
	this->zStacking = other->zStacking;
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