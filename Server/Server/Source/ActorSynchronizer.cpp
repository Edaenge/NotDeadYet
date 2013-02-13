#include "ActorSynchronizer.h"
#include "Actor.h"
#include <Packets\ServerFramePacket.h>


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
		zUpdateSet.insert(URE->zActor);
	}
	else if ( ActorScaleEvent* USE = dynamic_cast<ActorScaleEvent*>(e) )
	{
		zUpdateSet.insert(USE->zActor);
	}
	else if (ActorAdded* AD = dynamic_cast<ActorAdded*>(e))
	{
		zNewActorSet.insert(AD->zActor);
	}
	else if(ActorRemoved* AR = dynamic_cast<ActorRemoved*>(e))
	{
		zRemoveActorSet.insert(AR->zActor);
	}
}

void ActorSynchronizer::SendUpdatesTo( ClientData* cd )
{
	NetworkMessageConverter nmc;
	std::string msg;

	RegisterActor(cd);

	ServerFramePacket frameData;
	cd->Send(frameData);

	for(auto it = this->zUpdateSet.begin(); it != this->zUpdateSet.end(); it++)
	{
		frameData.newPositions[(*it)->GetID()] = (*it)->GetPosition();
		frameData.newRotations[(*it)->GetID()] = (*it)->GetRotation();
		frameData.newScales[(*it)->GetID()] = (*it)->GetScale();

		msg = nmc.Convert(MESSAGE_TYPE_UPDATE_ACTOR, (float)(*it)->GetID());
		msg += nmc.Convert(MESSAGE_TYPE_POSITION, (*it)->GetPosition());
		msg += nmc.Convert(MESSAGE_TYPE_ROTATION, (*it)->GetRotation());
		msg += nmc.Convert(MESSAGE_TYPE_SCALE, (*it)->GetScale());

		cd->Send(msg);
	}

	RemoveActor(cd);
}

void ActorSynchronizer::RegisterActor( ClientData* cd )
{
	NetworkMessageConverter nmc;
	std::string msg;

	for(auto it = zNewActorSet.begin(); it != this->zNewActorSet.end(); it++)
	{
		msg = nmc.Convert(MESSAGE_TYPE_NEW_ACTOR, (float)(*it)->GetID());
		msg += nmc.Convert(MESSAGE_TYPE_POSITION, (*it)->GetPosition());
		msg += nmc.Convert(MESSAGE_TYPE_ROTATION, (*it)->GetRotation());
		msg += nmc.Convert(MESSAGE_TYPE_SCALE, (*it)->GetScale());
		msg += nmc.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetModel());

		cd->Send(msg);
	}
}

void ActorSynchronizer::RemoveActor( ClientData* cd )
{
	NetworkMessageConverter nmc;
	std::string msg;

	for(auto it = zRemoveActorSet.begin(); it != this->zRemoveActorSet.end(); it++)
	{
		msg = nmc.Convert(MESSAGE_TYPE_REMOVE_ACTOR, (float)(*it)->GetID());

		cd->Send(msg);
	}
}

void ActorSynchronizer::ClearAll()
{
	this->zUpdateSet.clear();
	this->zNewActorSet.clear();
	
	//Deletes Actors
	auto i = this->zRemoveActorSet.begin();
	for(; i != this->zRemoveActorSet.end(); i++)
	{
		Actor* temp = (*i);
		SAFE_DELETE(temp);
	}
	this->zRemoveActorSet.clear();
}

