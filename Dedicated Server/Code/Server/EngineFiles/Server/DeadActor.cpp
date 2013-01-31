#include "DeadActor.h"

DeadActor::DeadActor( const bool genID /*= false*/ ) : Actor()
{
	if(genID)
		this->zID = this->GenerateID();

	this->zPos = Vector3(0, 0, 0);
	this->zRot = Vector4(0, 0, 0, 1);
	this->zScale = Vector3(0.05f, 0.05f, 0.05f);
}

DeadActor::~DeadActor()
{

}

Item* DeadActor::GetItemAt( const int Index ) const
{
	if ((unsigned int)Index >= this->zItems.size())
		return NULL;

	return this->zItems[Index];
}

Item* DeadActor::GetItem( const long ID ) const
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

std::string DeadActor::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg;

	msg = NMC->Convert(MESSAGE_TYPE_POSITION, zPos.x, zPos.y, zPos.z);
	msg += NMC->Convert(MESSAGE_TYPE_ROTATION, zRot.x, zRot.y, zRot.z, zRot.w);
	msg += NMC->Convert(MESSAGE_TYPE_SCALE, zScale.x, zScale.y, zScale.z);

	return msg;
}

