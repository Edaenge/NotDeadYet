#include "Thread.h"

Thread::Thread(const unsigned int id, const unsigned int itemType)
	:Material(id, itemType)
{
	this->zCraftingType = ITEM_TYPE_WEAPON_RANGED_BOW;
	this->zRequiredStackToCraft = 1;
}

Thread::~Thread()
{

}

bool Thread::Use()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
	{
		this->zStacks -= this->zRequiredStackToCraft;

		return true;
	}
	return false;
}

bool Thread::IsUsable()
{
	if (this->zStacks >= this->zRequiredStackToCraft)
		return true;

	return false;
}