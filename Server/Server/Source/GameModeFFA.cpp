#include "Game.h"
#include "GameModeFFA.h"
#include "BioActor.h"
#include "PlayerActor.h"

GameModeFFA::GameModeFFA(Game* game) : GameMode(game)
{

}

GameModeFFA::~GameModeFFA()
{

}

void GameModeFFA::OnEvent( Event* e )
{
	if( BioActorTakeDamageEvent* ATD = dynamic_cast<BioActorTakeDamageEvent*>(e) )
	{
		if( PlayerActor* pa = dynamic_cast<PlayerActor*>(ATD->zActor) )
		{
			if(!pa->IsAlive())
			{
			}
		}
	}
}
