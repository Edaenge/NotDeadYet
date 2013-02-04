#include "Material.h"

Material::Material(const long id, const unsigned int itemType, const unsigned int craftingType, const int stacksRequiredToCraft)
	: Item(id, itemType)
{
	this->zStacking = true;
	this->zCraftingType = craftingType;
	this->zRequiredStackToCraft = stacksRequiredToCraft;
}

Material::Material(const Material& other)
{
	this->zID = other.zID;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zCraftingType = other.zCraftingType;
	this->zItemDescription = other.zItemDescription;
	this->zRequiredStackToCraft = other.zRequiredStackToCraft;
}

Material::Material(const Material* other)
{
	this->zID = other->zID;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zCraftingType = other->zCraftingType;
	this->zItemDescription = other->zItemDescription;
	this->zRequiredStackToCraft = other->zRequiredStackToCraft;
}

Material::~Material()
{

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