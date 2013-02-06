#include "Game.h"
#include "GameMode.h"
#include "Behavior.h"
#include <world/World.h>


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
}


