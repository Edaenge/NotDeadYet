#include "GameModeFFA.h"
#include "Actor.h"
#include "Player.h"

GameModeFFA::GameModeFFA()
{

}

GameModeFFA::~GameModeFFA()
{

}

void GameModeFFA::OnEvent( Event* e )
{
	if ( PlayerConnectedEvent* PCE = dynamic_cast<PlayerConnectedEvent*>(e) )
	{

	}
}
