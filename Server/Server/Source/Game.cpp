#include "Game.h"


Game::Game( GameMode* mode, World* world ) : zGameMode(mode), zWorld(world)
{
}

bool Game::Update( float dt )
{
	// TODO: Update Behaviors
	// TODO: Update Game Mode
	// TODO: Update World

	return true;
}

void Game::OnEvent( Event* e )
{
	// TODO: Player Connected
	// TODO: Player Disconnected
	// TODO: Incoming Message
}


