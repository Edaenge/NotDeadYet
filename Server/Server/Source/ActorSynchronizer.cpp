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

	for(auto it = this->zUpdateSet.begin(); it != this->zUpdateSet.end(); it++)
	{
		msg = nmc.Convert(MESSAGE_TYPE_UPDATE_ACTOR, (*it)->GetID());
		msg += nmc.Convert(MESSAGE_TYPE_POSITION, (*it)->GetPosition());
		msg += nmc.Convert(MESSAGE_TYPE_ROTATION, (*it)->GetRotation());
		msg += nmc.Convert(MESSAGE_TYPE_SCALE, (*it)->GetScale());

		cd->Send(msg);
	}
}

void ActorSynchronizer::ClearAll()
{
	this->zUpdateSet.clear();
}