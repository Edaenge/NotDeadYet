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

DeadPlayerObjectActor::DeadPlayerObjectActor(const bool genID /*= false*/) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zPos = Vector3(0, 0, 0);
	this->zRot = Vector4(0, 0, 0, 1);
	this->zScale = Vector3(0.05f, 0.05f, 0.05f);
}