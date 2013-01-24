#include "DeadPlayerObjectActor.h"

Item* DeadPlayerObjectActor::GetItem(const long ID) const
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

Item* DeadPlayerObjectActor::GetItemAt( const int Index ) const
{
	if (Index >= this->zItems.size())
		return NULL;

	return this->zItems[Index];
}