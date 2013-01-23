#include "Material.h"

Material::Material(const unsigned int id, const unsigned int itemType, const unsigned int craftingType, const int stacksRequiredToCraft)
	: Item(id, itemType)
{
	this->zStacking = true;
	this->zCraftingType = craftingType;
	this->zRequiredStackToCraft = stacksRequiredToCraft;
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