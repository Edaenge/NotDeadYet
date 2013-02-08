#include "Game.h"
#include "time.h"
#include "GameModeFFA.h"
#include "BioActor.h"
#include "PlayerActor.h"
#include "GameEvents.h"

GameModeFFA::GameModeFFA(Game* game, int killLimit) : GameMode(game)
{
	srand(time(0));
	zKillLimit = killLimit;
}

GameModeFFA::~GameModeFFA()
{

}

bool GameModeFFA::Update( float dt )
{
	for(auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		if(zScoreBoard[(*it)] >= zKillLimit)
		{
			return false;
		}
	}
	return true;
}


void GameModeFFA::OnEvent( Event* e )
{
	if( BioActorTakeDamageEvent* ATD = dynamic_cast<BioActorTakeDamageEvent*>(e) )
	{
		if( PlayerActor* pa = dynamic_cast<PlayerActor*>(ATD->zActor) )
		{
			if(!pa->IsAlive())
			{
				// Set new spawn pos
				int maxPlayers = zGame->GetMaxNrOfPlayers();
				int rand = 1 + std::rand()%maxPlayers;
				pa->SetPosition(zGame->CalcPlayerSpawnPoint(rand));

				//Add to scoreboard
				if( PlayerActor* dpa = dynamic_cast<PlayerActor*>(ATD->zDealer) )
					if(dpa != pa)
						zScoreBoard[dpa->GetPlayer()]++;
					else if( dpa == pa )
						zScoreBoard[pa->GetPlayer()]--;
			}
		}
	}
	else if( PlayerAddEvent* PAE = dynamic_cast<PlayerAddEvent*>(e) )
	{
		this->zScoreBoard[PAE->player] = 0;
		zPlayers.insert(PAE->player);
	}
	else if( PlayerRemoveEvent* PRE = dynamic_cast<PlayerRemoveEvent*>(e) )
	{
		this->zScoreBoard.erase(PRE->player);
		this->zPlayers.erase(PRE->player);
	}
}