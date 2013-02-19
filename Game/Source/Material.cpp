#include "Material.h"

Material::Material()
{
	this->zStacking = true;
	this->zCraftingType = 10;
	this->zRequiredStackToCraft = 50;
}

Material::Material(const unsigned int itemType, const unsigned int itemSubType, 
				   const unsigned int craftingType, const unsigned int stacksRequiredToCraft) 
				   : Item(itemType, itemSubType)
{
	this->zStacking = true;
	this->zCraftingType = craftingType;
	this->zRequiredStackToCraft = stacksRequiredToCraft;
}

Material::Material( const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType, const unsigned int craftingType, const unsigned int stacksRequiredToCraft )
	: Item(ID, itemType, itemSubType)
{
	this->zStacking = true;
	this->zCraftingType = craftingType;
	this->zRequiredStackToCraft = stacksRequiredToCraft;
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
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zCraftingType = other.zCraftingType;
	this->zItemDescription = other.zItemDescription;
	this->zRequiredStackToCraft = other.zRequiredStackToCraft;
	this->zStacking = other.zStacking;
}

Material::Material(const Material* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zMeshModel = other->zMeshModel;
	this->zItemSubType = other->zItemSubType;
	this->zCraftingType = other->zCraftingType;
	this->zItemDescription = other->zItemDescription;
	this->zRequiredStackToCraft = other->zRequiredStackToCraft;
	this->zStacking = other->zStacking;
}

bool Material::Use()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
	{
		this->zStacks -= this->zRequiredStackToCraft;

		return true;
	}
	return false;
}

bool Material::IsUsable()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
		return true;

	return false;
}

std::string Material::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);

	msg += NMC->Convert(MESSAGE_TYPE_MATERIAL_STACKS_REQUIRED, (float)this->zRequiredStackToCraft);
	msg += NMC->Convert(MESSAGE_TYPE_MATERIAL_CRAFTING_TYPE, (float)this->zCraftingType);
	
	return msg;
}