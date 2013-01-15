#include "SmallStick.h"
#include "Projectile.h"

SmallStick::SmallStick(const unsigned int id, const unsigned int itemType)
	: Material(id, itemType)
{
	this->zCraftingType = ITEM_TYPE_PROJECTILE_ARROW;
}

SmallStick::~SmallStick()
{

}

bool SmallStick::Use()
{
	if (this->zStacks > 2)
	{
		return true;
	}
	return false;
}