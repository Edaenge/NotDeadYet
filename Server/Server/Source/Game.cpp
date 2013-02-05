#include "Game.h"
#include "GameMode.h"
#include <world/World.h>


Game::Game( GameMode* mode, World* world ) : zGameMode(mode), zWorld(world)
{
}

bool Game::Update( float dt )
{
	// TODO: Update Behaviors

	zGameMode->update(dt);
	zWorld->Update();

	return true;
}

void Game::OnEvent( Event* e )
{
	// TODO: Player Connected
	// TODO: Player Disconnected
	// TODO: Incoming Message
}


