#include "DeadPlayerObject.h"

Item* DeadPlayerObject::GetItemAt(int Index) const
{
	if (Index >= this->zItems.size())
		return NULL;

	return this->zItems[Index];
}

Item* DeadPlayerObject::GetItem(const long ID) const
{
	for (auto it = this->zItems.begin(); it < this->zItems.end(); it++)
	{
		if ((*it)->GetID() == ID)
		{
			return (*it);
		}
	}
	return NULL;
}