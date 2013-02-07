#include "Game.h"
#include "GameMode.h"
#include "Behavior.h"
#include <world/World.h>
#include "GameEvents.h"
#include "PlayerHumanBehavior.h"
#include "PlayerWolfBehavior.h"
#include "PlayerActor.h""
#include "PlayerWolfBehavior.h"
#include "AIWolfBehavior.h"
#include "WorldActor.h"
#include <World/EntityList.h>


Game::Game(ActorSynchronizer* syncher, GameMode* mode, const std::string& worldFile ) : zGameMode(mode)
{
	// Load Entities
	LoadEntList("Entities.txt");

	// Game Mode Observes
	AddObserver(zGameMode);

	// Create World
	zWorld = new World(this, worldFile.c_str());

	// Actor Manager
	zActorManager = new ActorManager(syncher);
}

Game::~Game()
{
	if ( zWorld ) delete zWorld, zWorld = 0;

	SAFE_DELETE(zActorManager);
}

bool Game::Update( float dt )
{
	// Update Behaviors
	auto i = zBehaviors.begin();
	while( i != zBehaviors.end() )
	{
		if ( (*i)->Update(dt) )
		{
			i = zBehaviors.erase(i);
		}
		else
		{
			++i;
		}
	}

	// Update Game Mode, Might Notify That GameMode is Finished
	if ( !zGameMode->Update(dt) )
		return false;
	
	// Update World
	zWorld->Update();

	// Game Still Active
	return true;
}

void Game::OnEvent( Event* e )
{
	// TODO: Incoming Message

	// World Loaded Event
	if ( PlayerConnectedEvent* PCE = dynamic_cast<PlayerConnectedEvent*>(e) )
	{
		// Create Player
		Player* player = new Player(PCE->clientData);
		zPlayers[PCE->clientData] = player;
	}
	else if( KeyDownEvent* KDE = dynamic_cast<KeyDownEvent*>(e) )
	{
		zPlayers[KDE->clientData]->GetKeys().SetKeyState(KDE->key, true);
	}
	else if( KeyUpEvent* KUE = dynamic_cast<KeyUpEvent*>(e) )
	{
		zPlayers[KUE->clientData]->GetKeys().SetKeyState(KUE->key, false);
	}
	else if( ClientDataEvent* CDE = dynamic_cast<ClientDataEvent*>(e) )
	{
		if( PlayerBehavior* dCastBehavior = dynamic_cast<PlayerBehavior*>(zPlayers[CDE->clientData]->GetBehavior()))
			dCastBehavior->ProcessClientData(CDE->direction, CDE->rotation);
	}
	else if ( PlayerKickEvent* PDCE = dynamic_cast<PlayerKickEvent*>(e) )
	{
		// Delete Player Behavior
		auto playerIterator = zPlayers.find(PDCE->clientData);
		auto playerBehavior = playerIterator->second->GetBehavior();

		// Create AI Behavior For Players That Disconnected
		if ( PlayerWolfBehavior* playerWolf = dynamic_cast<PlayerWolfBehavior*>(playerBehavior) )
		{
			AIWolfBehavior* aiWolf = new AIWolfBehavior(playerWolf->GetActor(), zWorld);
			SetPlayerBehavior(playerIterator->second, 0);
			zBehaviors.insert(aiWolf);
		}

		//Kick it
		PDCE->clientData->Kick();

		// Delete Player
		auto i = zPlayers.find(PDCE->clientData);
		delete i->second;
		zPlayers.erase(i);
	}
	else if ( PlayerPickupObjectEvent* PPOE = dynamic_cast<PlayerPickupObjectEvent*>(e))
	{
		
	}
	else if ( EntityUpdatedEvent* EUE = dynamic_cast<EntityUpdatedEvent*>(e) )
	{
		auto i = zWorldActors.find(EUE->entity);
		if ( i != zWorldActors.end() )
		{
			i->second->SetPosition(EUE->entity->GetPosition());
			// TODO: Rotation
			i->second->SetScale(EUE->entity->GetScale());
		}
	}
	else if ( EntityLoadedEvent* ELE = dynamic_cast<EntityLoadedEvent*>(e) )
	{
		PhysicsObject* phys = 0;
		
		if ( GetEntBlockRadius(ELE->entity->GetType()) > 0.0f )
		{
			phys = zPhysicsEngine->CreatePhysicsObject(GetEntModel(ELE->entity->GetType()), ELE->entity->GetPosition());
		}

		// Create Physics Object
		WorldActor* actor = new WorldActor();
		zWorldActors[ELE->entity] = actor;
		zActorManager->AddActor(actor);
	}
	else if ( EntityRemovedEvent* ERE = dynamic_cast<EntityRemovedEvent*>(e) )
	{
		auto i = zWorldActors.find(ERE->entity);
		if ( i != zWorldActors.end() )
		{
			delete i->second;
			zWorldActors.erase(i);
		}
	}
	else if ( UserDataEvent* UDE = dynamic_cast<UserDataEvent*>(e) )
	{
		// Create Player Actor
		PhysicsObject* pObject = this->zPhysicsEngine->CreatePhysicsObject(UDE->playerModel);
		Actor* actor = new PlayerActor(zPlayers[UDE->clientData], pObject);
		actor->SetPosition(Vector3(50, 0, 50));
		actor->SetActorModel(UDE->playerModel);
		zActorManager->AddActor(actor);
		// Apply Default Player Behavior
		SetPlayerBehavior(zPlayers[UDE->clientData], new PlayerHumanBehavior(actor, zWorld, zPlayers[UDE->clientData]));	
	}
	else if ( WorldLoadedEvent* WLE = dynamic_cast<WorldLoadedEvent*>(e) )
	{

	}

	NotifyObservers(e);
}

void Game::SetPlayerBehavior( Player* player, PlayerBehavior* behavior )
{
	// Find Old Behavior
	Behavior* curPlayerBehavior = player->GetBehavior();

	// Find In Behaviors
	if ( curPlayerBehavior )
	{
		zBehaviors.erase(curPlayerBehavior);
		delete curPlayerBehavior;
	}

	// Set New Behavior
	if ( behavior )	zBehaviors.insert(behavior);
	player->zBehavior = behavior;
}

