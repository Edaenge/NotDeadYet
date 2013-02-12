#include "Game.h"
#include "time.h"
#include "GameModeFFA.h"
#include "BioActor.h"
#include "PlayerActor.h"
#include "GameEvents.h"

GameModeFFA::GameModeFFA(Game* game, int killLimit) : GameMode(game)
{
	srand((unsigned int)time(0));
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
			for(auto i = zPlayers.begin(); i != zPlayers.end(); i++)
			{
				MaloW::Debug("Kills: " + MaloW::convertNrToString((float)zScoreBoard[(*i)]));
			}
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
				int maxPlayers = zPlayers.size();
				int rand = 1 + std::rand()%maxPlayers;
				pa->SetPosition(zGame->CalcPlayerSpawnPoint(rand));

				//Add to scoreboard
				if( PlayerActor* dpa = dynamic_cast<PlayerActor*>(ATD->zDealer) )
					if(dpa != pa)
						zScoreBoard[dpa->GetPlayer()]++;
					else if( dpa == pa )
						zScoreBoard[pa->GetPlayer()]--;
			}
			unsigned int ID = ATD->zDealer->GetID();
			float damage = ATD->zDamage.GetTotal();
			NetworkMessageConverter NMC;
			std::string msg = NMC.Convert(MESSAGE_TYPE_ACTOR_TAKE_DAMAGE, (float)ID);
			msg += NMC.Convert(MESSAGE_TYPE_HEALTH, damage);
			ClientData* cd = pa->GetPlayer()->GetClientData();
			cd->Send(msg);
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