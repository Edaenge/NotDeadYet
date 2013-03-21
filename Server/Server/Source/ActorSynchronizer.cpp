#include "ActorSynchronizer.h"
#include "Actor.h"
#include "BioActor.h"
#include "GhostActor.h"
#include "PlayerActor.h"
#include <Packets\ServerFramePacket.h>
#include <Packets\NewActorPacket.h>
#include <Packets\PhysicalConditionPacket.h>
#include "WorldActor.h"
#include "AnimationFileReader.h"
#include <time.h>
#include "ActorManager.h"

ActorSynchronizer::ActorSynchronizer()
{
	this->zFrameData = new ServerFramePacket();
	this->zActorData = new NewActorPacket();

	this->zAnimManager = new AnimationManager();
}

ActorSynchronizer::~ActorSynchronizer()
{
	SAFE_DELETE(this->zFrameData);
	SAFE_DELETE(this->zActorData);
	SAFE_DELETE(this->zAnimManager);

	auto it_phys_end = this->zIndividualPhysicalConditions.end();
	for ( auto it = this->zIndividualPhysicalConditions.begin(); it != it_phys_end; it++ )
	{
		if( it->second )
		{
			PhysicalConditionPacket* temp = it->second;
			SAFE_DELETE(temp);
		}
	}
}

void ActorSynchronizer::AddAnimation(BioActor* bActor)
{
	AnimationQueue queue;

	if(bActor->GetPlayer() != NULL)
	{
		queue = this->zAnimManager->CreatePlayerAnimationQueue(bActor);
	}
	else
	{
		queue = this->zAnimManager->CreateAnimalAnimationQueue(bActor);
	}

	if (!queue.empty())
		this->zFrameData->newAnimQueue[bActor->GetID()] = queue;
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
	else if (ActorChangedModelEvent* AMCE = dynamic_cast<ActorChangedModelEvent*>(e))
	{
		this->zFrameData->newMesh[AMCE->zActor->GetID()] = AMCE->zActor->GetModel();
	}
	else if (BioActorStateEvent* BASE = dynamic_cast<BioActorStateEvent*>(e))
	{
		BioActor* bActor =  dynamic_cast<BioActor*>(BASE->zBioActor);
		this->zFrameData->newStates[BASE->zBioActor->GetID()] = bActor->GetState();
	
		this->AddAnimation(bActor);
	}
	else if (ActorAddedEvent* AD = dynamic_cast<ActorAddedEvent*>(e))
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
	else if(ActorRemovedEvent* AR = dynamic_cast<ActorRemovedEvent*>(e))
	{
		// Do Not Sync World Actors
		if( WorldActor* WA = dynamic_cast<WorldActor*>(AR->zActor) )
			return;

		AR->zActor->RemoveObserver(this);

		// Entity Removed, Do Not Send Created Event
		zNewActorSet.erase(AR->zActor);
		zRemoveActorSet.insert(AR->zActor->GetID());
	}
	//Condition Events
	else if(ActorPhysicalConditionEnergyEvent* APCE = dynamic_cast<ActorPhysicalConditionEnergyEvent*>(e))
	{
		ClientData* cd = NULL;
		Player* player = NULL;

		if( BioActor* bioActor = dynamic_cast<BioActor*>(APCE->zActor) )
		{
			player = bioActor->GetPlayer();

			if( !player )
				return;

			cd = player->GetClientData();
		}
		else if( GhostActor* ghostActor = dynamic_cast<GhostActor*>(APCE->zActor) )
			cd = ghostActor->GetPlayer()->GetClientData();
		

		if( !cd )
			return;

		auto found = this->zIndividualPhysicalConditions.find(cd);
		if( found == this->zIndividualPhysicalConditions.end() )
			this->zIndividualPhysicalConditions[cd] = new PhysicalConditionPacket();

		this->zIndividualPhysicalConditions[cd]->zEnergy = APCE->zActor->GetEnergy();

	}
	else if( BioActorPhysicalConditionHealthEvent* BAPCH = dynamic_cast<BioActorPhysicalConditionHealthEvent*>(e) )
	{
		ClientData* cd = NULL;
		Player* player = NULL;
		BioActor* bioActor = dynamic_cast<BioActor*>(BAPCH->zBioActor);

		if( bioActor )
		{
			player = bioActor->GetPlayer();

			if( !player )
				return;

			cd = player->GetClientData();
		}
		
		if( !cd )
			return;
		
		auto found = this->zIndividualPhysicalConditions.find(cd);
		if( found == this->zIndividualPhysicalConditions.end() )
			this->zIndividualPhysicalConditions[cd] = new PhysicalConditionPacket();

		this->zIndividualPhysicalConditions[cd]->zHealth = bioActor->GetHealth();
	}
	else if ( BioActorPhysicalConditionStaminaEvent* BAPCS = dynamic_cast<BioActorPhysicalConditionStaminaEvent*>(e) )
	{
		ClientData* cd = NULL;
		Player* player = NULL;
		BioActor* bioActor = dynamic_cast<BioActor*>(BAPCS->zBioActor);

		if( bioActor )
		{
			player = bioActor->GetPlayer();

			if( !player )
				return;

			cd = player->GetClientData();
		}
		
		if( !cd )
			return;

		auto found = this->zIndividualPhysicalConditions.find(cd);
		if( found == this->zIndividualPhysicalConditions.end() )
			this->zIndividualPhysicalConditions[cd] = new PhysicalConditionPacket();

		this->zIndividualPhysicalConditions[cd]->zStamina = bioActor->GetStamina();
	}
	else if ( BioActorPhysicalConditionBleedingEvent* BAPCB = dynamic_cast<BioActorPhysicalConditionBleedingEvent*>(e) )
	{
		ClientData* cd = NULL;
		Player* player = NULL;
		BioActor* bioActor = dynamic_cast<BioActor*>(BAPCB->zBioActor);

		if( bioActor )
		{
			player = bioActor->GetPlayer();

			if( !player )
				return;

			cd = player->GetClientData();
		}

		if( !cd )
			return;

		auto found = this->zIndividualPhysicalConditions.find(cd);
		if( found == this->zIndividualPhysicalConditions.end() )
			this->zIndividualPhysicalConditions[cd] = new PhysicalConditionPacket();

		this->zIndividualPhysicalConditions[cd]->zBleedingLevel = bioActor->GetBleeding();

	}
	else if( PlayerActorPhysicalConditionHungerEvent* PAPCH = dynamic_cast<PlayerActorPhysicalConditionHungerEvent*>(e) )
	{
		ClientData* cd = NULL;
		Player* player = NULL;
		PlayerActor* playerActor = dynamic_cast<PlayerActor*>(PAPCH->zPlayerActor);

		if( playerActor )
		{
			player = playerActor->GetPlayer();

			if( !player )
				return;

			cd = player->GetClientData();
		}

		if( !cd )
			return;

		auto found = this->zIndividualPhysicalConditions.find(cd);
		if( found == this->zIndividualPhysicalConditions.end() )
			this->zIndividualPhysicalConditions[cd] = new PhysicalConditionPacket();

		this->zIndividualPhysicalConditions[cd]->zHunger = playerActor->GetFullness();
	}
	else if( PlayerActorPhysicalConditionHydrationEvent* PAPCHe = dynamic_cast<PlayerActorPhysicalConditionHydrationEvent*>(e) )
	{
		ClientData* cd = NULL;
		Player* player = NULL;
		PlayerActor* playerActor = dynamic_cast<PlayerActor*>(PAPCHe->zPlayerActor);

		if( playerActor )
		{
			player = playerActor->GetPlayer();

			if( !player )
				return;

			cd = player->GetClientData();
		}

		if( !cd )
			return;

		auto found = this->zIndividualPhysicalConditions.find(cd);
		if( found == this->zIndividualPhysicalConditions.end() )
			this->zIndividualPhysicalConditions[cd] = new PhysicalConditionPacket();

		this->zIndividualPhysicalConditions[cd]->zHydration = playerActor->GetHydration();
	}
}

void ActorSynchronizer::SendUpdatesTo( ClientData* cd )
{
	cd->Send(*zActorData);

	auto found = zIndividualPhysicalConditions.find(cd);
	if( found != zIndividualPhysicalConditions.end() )
		cd->Send(*found->second);

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
	auto it_phys_end = this->zIndividualPhysicalConditions.end();
	for ( auto it = this->zIndividualPhysicalConditions.begin(); it != it_phys_end; it++ )
	{
		if( it->second )
		{
			PhysicalConditionPacket* temp = it->second;
			delete temp;
		}
	}

	// Clear Sets
	this->zIndividualPhysicalConditions.clear();
	this->zUpdateSet.clear();
	this->zNewActorSet.clear();
	this->zRemoveActorSet.clear();
}