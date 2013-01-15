#include "SmallStick.h"
#include "Projectile.h"

SmallStick::SmallStick(const unsigned int id, const unsigned int itemType)
	: Material(id, itemType)
{
	this->zCraftingType = ITEM_TYPE_PROJECTILE_ARROW;
	this->zRequiredStackToCraft = 2;
}

SmallStick::~SmallStick()
{

}

bool SmallStick::Use()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
	{
		this->zStacks -= this->zRequiredStackToCraft;

		return true;
	}
	return false;
}

bool SmallStick::IsUsable()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
		return true;

	return false;
}