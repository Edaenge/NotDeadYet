#include "Game.h"
#include "GameMode.h"
#include "Behavior.h"
#include <world/World.h>
#include "GameEvents.h"
#include "PlayerBehavior.h"
#include "PlayerWolfBehavior.h"
#include "PlayerActor.h"


Game::Game(ActorSynchronizer* syncher, GameMode* mode, const std::string& worldFile ) : zGameMode(mode)
{
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
}

bool Game::Update( float dt )
{
	// Update Behaviors
	auto i = zBehaviors.begin();
	while( i != zBehaviors.end() )
	{
		if ( i->second->Update(dt) )
		{
			i = zBehaviors.erase(i);
		}
		else
		{
			++i;
		}
	}

	// Update Game Mode
	if ( !zGameMode->Update(dt) )
		return false;
	
	// Update World
	zWorld->Update();

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

		// Create Player Actor
		Actor* actor = new PlayerActor(player);
		zActorManager->AddActor(actor);

		// Apply Default Player Behavior
		//zBehaviors[player] = new PlayerHumanBehavior(actor, zWorld, player);
	}
	else if( KeyDownEvent* KDE = dynamic_cast<KeyDownEvent*>(e) )
	{
		zPlayers[KDE->clientData]->GetKeys().SetKeyState(KDE->key, true);
	}
	else if( KeyUpEvent* KUE = dynamic_cast<KeyUpEvent*>(e) )
	{
		zPlayers[KDE->clientData]->GetKeys().SetKeyState(KDE->key, false);
	}
	else if( ClientDataEvent* CDE = dynamic_cast<ClientDataEvent*>(e) )
	{
		Behavior* behavior = zBehaviors[zPlayers[CDE->clientData]];
		if( PlayerBehavior* dCastBehavior = dynamic_cast<PlayerBehavior*>(behavior))
			int i = 0;
		//_behaviors[_players[CDE->clientData]]->ProcessClientData(CDE->direction, CDE->direction);
	}
	else if ( PlayerDisconnectedEvent* PDCE = dynamic_cast<PlayerDisconnectedEvent*>(e) )
	{
		// TODO: Delete Player Behavior

		// Delete Player
		auto i = zPlayers.find(PDCE->clientData);
		delete i->second;
		zPlayers.erase(i);
	}
	else if ( WorldLoadedEvent* WLE = dynamic_cast<WorldLoadedEvent*>(e) )
	{

	}

	NotifyObservers(e);
}


