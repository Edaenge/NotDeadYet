#include "ActorSynchronizer.h"

ActorSynchronizer::ActorSynchronizer()
{

}

ActorSynchronizer::~ActorSynchronizer()
{

}

void ActorSynchronizer::OnEvent( Event* e )
{
	if ( ActorPositionEvent* UPE = dynamic_cast<ActorPositionEvent*>(e) )
	{
		zUpdateSet.insert(UPE->zActor);
	}
	else if ( ActorRotationEvent* URE = dynamic_cast<ActorRotationEvent*>(e) )
	{
		zUpdateSet.insert(UPE->zActor);
	}
	else if ( ActorScaleEvent* USE = dynamic_cast<ActorScaleEvent*>(e) )
	{
		zUpdateSet.insert(UPE->zActor);
	}
	}}

void ActorSynchronizer::SendUpdatesTo( ClientData* cd )
{
	NetworkMessageConverter* nmc = new NetworkMessageConverter();
	std::string msg;
	Vector3 pos;
	Vector4 rot;
	Vector3 scale;

	for(auto it = this->zUpdateSet.begin(); it != this->zUpdateSet.end(); it++)
	{
		pos = (*it)->GetPosition();
		rot = (*it)->GetRotation();
		scale = (*it)->GetScale();

		msg = nmc->Convert(MESSAGE_TYPE_UPDATE_PLAYER, cd->GetClient()->GetClientID());
		msg += nmc->Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		msg += nmc->Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		msg += nmc->Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);

		cd->SendMessage(msg);
	}
	return true;
}

void ActorSynchronizer::ClearAll()
{
	this->zUpdateSet.clear();
	return true;
}