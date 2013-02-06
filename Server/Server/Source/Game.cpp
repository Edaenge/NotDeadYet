#include "Game.h"
#include "GameMode.h"
#include "Behavior.h"
#include <world/World.h>
#include "GameEvents.h"


Game::Game(ActorSynchronizer* syncher, GameMode* mode, const std::string& worldFile ) : zGameMode(mode)
{
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
	auto i = _behaviors.begin();
	while( i != _behaviors.end() )
	{
		if ( i->second->Update(dt) )
		{
			i = _behaviors.erase(i);
		}
		else
		{
			++i;
		}
	}

	// Update Game Mode
	zGameMode->Update(dt);
	
	// Update World
	zWorld->Update();

	return true;
}

void Game::OnEvent( Event* e )
{
	// TODO: Player Connected
	// TODO: Player Disconnected
	// TODO: Incoming Message

	// World Loaded Event
	if ( WorldLoadedEvent* WLE = dynamic_cast<WorldLoadedEvent*>(e) )
	{

	}
	else if( KeyDownEvent* KDE = dynamic_cast<KeyDownEvent*>(e) )
	{
		Player* temp = _players[KDE->clientData];
		_keyStates[temp].SetKeyState(KDE->key, true);
	}
	else if( KeyUpEvent* KDE = dynamic_cast<KeyUpEvent*>(e) )
	{
		Player* temp = _players[KDE->clientData];
		_keyStates[temp].SetKeyState(KDE->key, false);
	}
}


