#include "MediumStick.h"

MediumStick::MediumStick(const unsigned int id, const unsigned int itemType) 
	: Material(id, itemType)
{
	this->zCraftingType = ITEM_TYPE_WEAPON_RANGED_BOW;
	this->zRequiredStackToCraft = 4;
}

MediumStick::~MediumStick()
{

}

bool MediumStick::Use()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
	{
		this->zStacks -= this->zRequiredStackToCraft;

		return true;
	}
	return false;
}

bool MediumStick::IsUsable()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
		return true;

	return false;
}