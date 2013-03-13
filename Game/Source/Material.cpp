#include "Material.h"

Material::Material()
{
	this->zStacking = true;
}

Material::Material(const unsigned int itemType, const unsigned int itemSubType) 
				   : Item(itemType, itemSubType)
{
	this->zStacking = true;
}

Material::Material( const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType)
	: Item(ID, itemType, itemSubType)
{
	this->zStacking = true;
}

Material::~Material()
{

}

Material::Material(const Material& other)
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

bool Material::Use()
{
	return true;
}

std::string Material::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);
	
	return msg;
}