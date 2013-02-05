#include "Game.h"
#include "GameMode.h"
#include <world/World.h>


Game::Game( GameMode* mode, const std::string& worldFile ) : zGameMode(mode)
{
	// Create World
	zWorld = new World(this, worldFile.c_str());
}

Game::~Game()
{
	if ( zWorld ) delete zWorld, zWorld = 0;
}

bool Game::Update( float dt )
{
	// TODO: Update Behaviors
	zGameMode->Update(dt);
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


