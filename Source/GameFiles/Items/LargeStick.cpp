#include "LargeStick.h"

LargeStick::LargeStick(const unsigned int id, const unsigned int itemType)
	: Material(id, itemType)
{
	this->zCraftingType = ITEM_TYPE_WEAPON_RANGED_BOW;
	this->zRequiredStackToCraft = 100;
}

LargeStick::~LargeStick()
{

}

bool LargeStick::Use()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
	{
		this->zStacks -= this->zRequiredStackToCraft;

		return true;
	}
	return false;
}

bool LargeStick::IsUsable()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
		return true;

	return false;
}