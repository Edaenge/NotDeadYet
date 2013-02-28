#include "ActorSynchronizer.h"
#include "Actor.h"
#include "BioActor.h"
#include <Packets\ServerFramePacket.h>
#include <Packets\NewActorPacket.h>
#include "WorldActor.h"
#include "AnimationFileReader.h"
#include <time.h>

ActorSynchronizer::ActorSynchronizer()
{
	this->zFrameData = new ServerFramePacket();
	this->zActorData = new NewActorPacket();
}

ActorSynchronizer::~ActorSynchronizer()
{
	SAFE_DELETE(this->zFrameData);
	SAFE_DELETE(this->zActorData);
}

void ActorSynchronizer::AddAnimation(BioActor* bActor)
{
	KeyStates keys = bActor->GetPlayer()->GetKeys();
	unsigned int state = bActor->GetState();
	std::string animation = "";
	if (state == STATE_IDLE)
	{
		srand((unsigned int)time(0));

		int idle_Animation = (rand() % 1000) + 1;
		if (idle_Animation > 0 && idle_Animation <= 160)//High Chance 16%
			animation = IDLE_O1;
		else if (idle_Animation > 160 && idle_Animation <= 270)//Medium Chance 11%
			animation = IDLE_O2;
		else if (idle_Animation > 270 && idle_Animation <= 340)//Low Chance 7%
			animation = IDLE_O3;
		else if (idle_Animation > 340 && idle_Animation <= 590)//Very High Chance 25%
			animation = IDLE_O4;
		else if (idle_Animation > 590 && idle_Animation <= 840)//Very High Chance 25%
			animation = IDLE_O5;
		else if (idle_Animation > 840 && idle_Animation <= 1000)//High Chance 16%
			animation = IDLE_O6;

	}
	else if (state == STATE_WALKING)
	{
		if(keys.GetKeyState(KEY_FORWARD))
			animation = WALK_FORWARD;
		else if (keys.GetKeyState(KEY_BACKWARD))
			animation = WALK_BACKWARD;
		else if(keys.GetKeyState(KEY_LEFT))
			animation = WALK_LEFT;
		else if (keys.GetKeyState(KEY_RIGHT))
			animation = WALK_RIGHT;
	}
	//else if (state == STATE_JOG)
	//{
	//}
	else if (state == STATE_RUNNING)
		animation = SPRINT;

	if (animation != "")
		this->zFrameData->newAnimations[bActor->GetID()] = animation;
}

void ActorSynchronizer::OnEvent( Event* e )
{
	if (ActorPositionEvent* UPE = dynamic_cast<ActorPositionEvent*>(e))
	{
		this->zFrameData->newPositions[UPE->zActor->GetID()] = UPE->zActor->GetPosition();
	}
	else if (ActorRotationEvent* URE = dynamic_cast<ActorRotationEvent*>(e))
	{
		this->zFrameData->newRotations[URE->zActor->GetID()] = URE->zActor->GetRotation();
	}
	else if (ActorScaleEvent* USE = dynamic_cast<ActorScaleEvent*>(e))
	{
		this->zFrameData->newScales[USE->zActor->GetID()] = USE->zActor->GetScale();
	}
	else if (BioActorStateEvent* BASE = dynamic_cast<BioActorStateEvent*>(e))
	{
		BioActor* bActor =  dynamic_cast<BioActor*>(BASE->zBioActor);
		this->zFrameData->newStates[BASE->zBioActor->GetID()] = bActor->GetState();
	
		this->AddAnimation(bActor);
	}
	else if (ActorAdded* AD = dynamic_cast<ActorAdded*>(e))
	{
		// Do Not Sync World Actors
		if( WorldActor* WA = dynamic_cast<WorldActor*>(AD->zActor) )
			return;

		this->zActorData->actorPosition[AD->zActor->GetID()] = AD->zActor->GetPosition();
		this->zActorData->actorRotation[AD->zActor->GetID()] = AD->zActor->GetRotation();
		this->zActorData->actorScale[AD->zActor->GetID()] = AD->zActor->GetScale();
		this->zActorData->actorModel[AD->zActor->GetID()] = AD->zActor->GetModel();
		if (BioActor* bActor = dynamic_cast<BioActor*>(AD->zActor))
			this->zActorData->actorState[AD->zActor->GetID()] = bActor->GetState();
		
		AD->zActor->AddObserver(this);
		//zNewActorSet.insert(AD->zActor);
	}
	else if(ActorRemoved* AR = dynamic_cast<ActorRemoved*>(e))
	{
		// Do Not Sync World Actors
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

	cd->Send(*zActorData);

	//RegisterActor(cd);
	
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
	// Clear packets
	if (this->zFrameData)
		delete this->zFrameData;

	zFrameData = new ServerFramePacket();

	if (this->zActorData)
		delete this->zActorData;

	zActorData = new NewActorPacket();

	// Clear Sets
	this->zUpdateSet.clear();
	this->zNewActorSet.clear();
	this->zRemoveActorSet.clear();
}

