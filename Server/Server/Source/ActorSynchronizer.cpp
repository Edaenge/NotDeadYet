#include "ActorSynchronizer.h"
#include "Actor.h"
#include "BioActor.h"
#include <Packets\ServerFramePacket.h>
#include "WorldActor.h"


ActorSynchronizer::ActorSynchronizer() : 
	zFrameData( new ServerFramePacket() )
{
}

ActorSynchronizer::~ActorSynchronizer()
{
	SAFE_DELETE(this->zFrameData);
}

void ActorSynchronizer::OnEvent( Event* e )
{
	if (ActorPositionEvent* UPE = dynamic_cast<ActorPositionEvent*>(e))
	{
		zFrameData->newPositions[UPE->zActor->GetID()] = UPE->zActor->GetPosition();
	}
	else if (ActorRotationEvent* URE = dynamic_cast<ActorRotationEvent*>(e))
	{
		zFrameData->newRotations[URE->zActor->GetID()] = URE->zActor->GetRotation();
	}
	else if (ActorScaleEvent* USE = dynamic_cast<ActorScaleEvent*>(e))
	{
		zFrameData->newScales[USE->zActor->GetID()] = USE->zActor->GetScale();
	}
	else if (BioActorStateEvent* BASE = dynamic_cast<BioActorStateEvent*>(e))
	{
		zFrameData->newStates[BASE->zBioActor->GetID()] = dynamic_cast<BioActor*>(BASE->zBioActor)->GetState();
	}
	else if (ActorAdded* AD = dynamic_cast<ActorAdded*>(e))
	{
		// Do Not Synch World Actors
		if( WorldActor* WA = dynamic_cast<WorldActor*>(AD->zActor) )
			return;

		AD->zActor->AddObserver(this);
		zNewActorSet.insert(AD->zActor);
	}
	else if(ActorRemoved* AR = dynamic_cast<ActorRemoved*>(e))
	{
		// Do Not Synch World Actors
		if( WorldActor* WA = dynamic_cast<WorldActor*>(AR->zActor) )
			return;

		AR->zActor->RemoveObserver(this);

		// Entity Removed, Do Not Send Created Event
		zNewActorSet.erase(AR->zActor);
		zRemoveActorSet.insert(AR->zActor->GetID());
	}
}

void ActorSynchronizer::SendUpdatesTo( ClientData* cd )
{
	NetworkMessageConverter nmc;
	std::string msg;

	RegisterActor(cd);
	
	cd->Send(*zFrameData);

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
		msg = nmc.Convert(MESSAGE_TYPE_REMOVE_ACTOR, (float)(*it));

		cd->Send(msg);
	}
}

void ActorSynchronizer::ClearAll()
{
	// Clear Frame Data
	delete zFrameData;
	zFrameData = new ServerFramePacket();

	// Clear Sets
	this->zUpdateSet.clear();
	this->zNewActorSet.clear();
	this->zRemoveActorSet.clear();
}

