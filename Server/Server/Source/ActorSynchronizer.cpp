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
		auto it = zUpdateSet.find(UPE->zActor);
		(*it)->SetPosition(UPE->zActor->GetPosition());
	}
	else if ( ActorRotationEvent* URE = dynamic_cast<ActorRotationEvent*>(e) )
	{
		auto it = zUpdateSet.find(UPE->zActor);
		(*it)->SetRotation(UPE->zActor->GetRotation());
	}
	else if ( ActorScaleEvent* USE = dynamic_cast<ActorScaleEvent*>(e) )
	{
		auto it = zUpdateSet.find(UPE->zActor);
		(*it)->SetScale(UPE->zActor->GetScale());
	}
}

void ActorSynchronizer::SendUpdatesTo( ClientData* cd )
{
	NetworkMessageConverter nmc;
	std::string msg;
	Vector3 pos;
	Vector4 rot;
	Vector3 scale;

	for(auto it = this->zUpdateSet.begin(); it != this->zUpdateSet.end(); it++)
	{
		pos = (*it)->GetPosition();
		rot = (*it)->GetRotation();
		scale = (*it)->GetScale();

		msg = nmc.Convert(MESSAGE_TYPE_UPDATE_ACTOR, (*it)->GetID());
		msg += nmc.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		msg += nmc.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		msg += nmc.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);

		cd->SendMessage(msg);
	}
}

void ActorSynchronizer::ClearAll()
{
	this->zUpdateSet.clear();
}