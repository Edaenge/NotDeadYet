#include "Game.h"
#include "time.h"
#include "GameModeFFA.h"
#include "BioActor.h"
#include "DeerActor.h"
#include "WolfActor.h"
#include "BearActor.h"
#include "PlayerActor.h"
#include "GhostActor.h"
#include "GameEvents.h"
#include "PlayerGhostBehavior.h"
#include "PlayerDeerBehavior.h"
#include "PlayerWolfBehavior.h"
#include "PlayerBearBehavior.h"

GameModeFFA::GameModeFFA(Game* game) : GameMode(game)
{
	srand((unsigned int)time(0));
	//zKillLimit = killLimit;
}

GameModeFFA::~GameModeFFA()
{
	this->zGame->RemoveObserver(this);
}

bool GameModeFFA::Update( float dt )
{
//	for(auto it = zPlayers.begin(); it != zPlayers.end(); it++)
//	{
//		if(zScoreBoard[(*it)] >= zKillLimit)
//		{
//			for(auto i = zPlayers.begin(); i != zPlayers.end(); i++)
//			{
//				MaloW::Debug("Kills: " + MaloW::convertNrToString((float)zScoreBoard[(*i)]));
//			}
//			return false;
//		}
//	}
	return true;
}

void GameModeFFA::OnEvent( Event* e )
{
	if( BioActorTakeDamageEvent* ATD = dynamic_cast<BioActorTakeDamageEvent*>(e) )
	{
		if( PlayerActor* pActor = dynamic_cast<PlayerActor*>(ATD->zActor) )
		{
			if(pActor->GetHealth() - ATD->zDamage->GetTotal() <= 0)
			{
				std::string killsMsg = "";
				Player* player = pActor->GetPlayer();
				if (ATD->zActor == ATD->zDealer)
				{
					killsMsg = player->GetPlayerName() + " Killed Himself";
				}
				else if (PlayerActor* pDealer = dynamic_cast<PlayerActor*>(ATD->zDealer))
				{
					Player* dealer = pDealer->GetPlayer();
					killsMsg = player->GetPlayerName() + " Was Killed by " + dealer->GetPlayerName();
				}
				else if (AnimalActor* pDealer =dynamic_cast<AnimalActor*>(ATD->zDealer))
				{
					killsMsg = player->GetPlayerName() + " Was killed by an Animal";
				}
				else
				{
					killsMsg = player->GetPlayerName() + " Was Killed by the environment";
				}

				NetworkMessageConverter NMC;
				std::string msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
				this->zGame->SendToAll(msg);

				this->OnPlayerHumanDeath(pActor);
			}
			else
			{
				unsigned int ID = ATD->zDealer->GetID();
				float damage = ATD->zDamage->GetTotal();

				NetworkMessageConverter NMC;
				std::string msg = NMC.Convert(MESSAGE_TYPE_ACTOR_TAKE_DAMAGE, (float)ID);
				msg += NMC.Convert(MESSAGE_TYPE_HEALTH, damage);
				ClientData* cd = pActor->GetPlayer()->GetClientData();
				cd->Send(msg);
			}
		}
		else if( AnimalActor* aActor = dynamic_cast<AnimalActor*>(ATD->zActor) )
		{
			if (aActor->GetPlayer())
			{
				if(aActor->GetHealth() - ATD->zDamage->GetTotal() <= 0)
				{
					this->OnPlayerAnimalDeath(aActor);
				}
				else
				{
					unsigned int ID = ATD->zDealer->GetID();
					float damage = ATD->zDamage->GetTotal();

					NetworkMessageConverter NMC;
					std::string msg = NMC.Convert(MESSAGE_TYPE_ACTOR_TAKE_DAMAGE, (float)ID);
					msg += NMC.Convert(MESSAGE_TYPE_HEALTH, damage);
					ClientData* cd = aActor->GetPlayer()->GetClientData();
					cd->Send(msg);
				}
			}
			else
			{
				if(aActor->GetHealth() - ATD->zDamage->GetTotal() <= 0)
				{
					this->zGame->RemoveAIBehavior(aActor);
				}
			}
			
		}
	}
	else if (PlayerAnimalSwapEvent* PASE = dynamic_cast<PlayerAnimalSwapEvent*>(e))
	{
		GhostActor* gActor = dynamic_cast<GhostActor*>(PASE->zActor);

		if (gActor)
			this->SwapToAnimal(gActor, PASE->zAnimalType);
	}
	else if( PlayerAddEvent* PAE = dynamic_cast<PlayerAddEvent*>(e) )
	{
		//this->zScoreBoard[PAE->player] = 0;
		zPlayers.insert(PAE->player);
	}
	else if( PlayerRemoveEvent* PRE = dynamic_cast<PlayerRemoveEvent*>(e) )
	{
		//this->zScoreBoard.erase(PRE->player);
		this->zPlayers.erase(PRE->player);
	}
}

void GameModeFFA::SwapToAnimal(GhostActor* gActor, unsigned int animalType)
{
	Player* player = gActor->GetPlayer();
	ClientData* cd = player->GetClientData();

	NetworkMessageConverter NMC;
	std::string msg = "";
	AnimalActor* closestAnimal = NULL;
	float distance = 999999.9f;
	
	Vector3 position = gActor->GetPosition();

	float radius = 4000.0f;
	std::set<Actor*> actors;
	this->zGame->GetActorManager()->GetActorsInCircle(position.GetXZ(), radius, actors, ACTOR_TYPE_ANIMAL);
	
	for (auto it_Actors = actors.begin(); it_Actors != actors.end(); it_Actors++)
	{
		if (AnimalActor* aActor = dynamic_cast<AnimalActor*>((*it_Actors)))
		{
			//If type = Deer
			if (animalType == 0)
			{
				if (DeerActor* dActor = dynamic_cast<DeerActor*>(aActor))
				{
					if (dActor->IsAlive())
					{
						float dist = (position - dActor->GetPosition()).GetLength();

						if (dist < distance)
						{
							distance = dist;
							closestAnimal = dActor;
						}
					}
				}
			}
			/*//If Type = Wolf
			else if (animalType == 1)
			{
				if (WolfActor* wActor = dynamic_cast<WolfActor*>(aActor))
				{
					//Kill player on Client
					Player* player = gActor->GetPlayer();
					gActor->SetPlayer(NULL);

					PlayerWolfBehavior* playerWolfBehavior = new PlayerWolfBehavior(wActor, this->zGame->GetWorld(), player);

					this->zGame->SetPlayerBehavior(player, playerWolfBehavior);

					wActor->SetPlayer(player);

					msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, wActor->GetID());
					msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

					cd->Send(msg);

					this->zGame->GetActorManager()->RemoveActor(gActor);
					found = true;
				}
			}*/
			//If Type = Bear
			else if (animalType == 2)
			{
				if (BearActor* bActor = dynamic_cast<BearActor*>(aActor))
				{
					if (bActor->IsAlive())
					{
						float dist = (position - bActor->GetPosition()).GetLength();

						if (dist < distance)
						{
							distance = dist;
							closestAnimal = bActor;
						}
					}
				}
			}
		}
	}

	PlayerBehavior* animalBehavior = NULL;
	if (closestAnimal)
	{
		//Deer
		if (animalType == 0)
		{
			animalBehavior = new PlayerDeerBehavior(closestAnimal, this->zGame->GetWorld(), player);

			gActor->SetPlayer(NULL);

			closestAnimal->SetPlayer(player);

			this->zGame->RemoveAIBehavior(closestAnimal);
			this->zGame->SetPlayerBehavior(player, animalBehavior);

			msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)closestAnimal->GetID());
			msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

			cd->Send(msg);

			this->zGame->GetActorManager()->RemoveActor(gActor);
		}
		//Bear
		else if (animalType == 2)
		{
			/*Player* player = gActor->GetPlayer();
			gActor->SetPlayer(NULL);

			PlayerBearBehavior* playerBearBehavior = new PlayerBearBehavior(dActor, this->zGame->GetWorld(), player);
			dActor->SetPlayer(player);

			this->zGame->SetPlayerBehavior(player, playerBearBehavior);

			msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)dActor->GetID());
			msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

			cd->Send(msg);

			this->zGame->GetActorManager()->RemoveActor(gActor);
			found = true;*/
			animalBehavior = new PlayerBearBehavior(closestAnimal, this->zGame->GetWorld(), player);

			gActor->SetPlayer(NULL);

			closestAnimal->SetPlayer(player);

			this->zGame->RemoveAIBehavior(closestAnimal);
			this->zGame->SetPlayerBehavior(player, animalBehavior);

			msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)closestAnimal->GetID());
			msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

			cd->Send(msg);

			this->zGame->GetActorManager()->RemoveActor(gActor);


		}
	}
	else
	{
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "No_animal_of_the_type_is_close_by"));
	}

}

void GameModeFFA::OnPlayerHumanDeath(PlayerActor* pActor)
{
	NetworkMessageConverter NMC;
	std::string msg = "";

	Player* player = pActor->GetPlayer();
	//Remove Player Pointer From the Actor
	pActor->SetPlayer(NULL);

	ClientData* cd = player->GetClientData();

	//Create Spirit
	Vector3 position = pActor->GetPosition();

	Vector3 direction = pActor->GetDir();

	GhostActor* gActor = new GhostActor(player);
	gActor->SetPosition(position);
	gActor->SetDir(direction);
	
	//Create Ghost behavior
	PlayerGhostBehavior* pGhostBehavior = new PlayerGhostBehavior(gActor, this->zGame->GetWorld(), player);

	this->zGame->SetPlayerBehavior(player, pGhostBehavior);

	//Tell Client his new ID and actor type
	ActorManager* aManager = this->zGame->GetActorManager();
	msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)gActor->GetID());
	msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 2);

	cd->Send(msg);
	
	//Add the actor to the list
	aManager->AddActor(gActor);
}

void GameModeFFA::OnPlayerAnimalDeath(AnimalActor* aActor)
{
	NetworkMessageConverter NMC;
	std::string msg = "";

	Player* player = aActor->GetPlayer();
	//Remove Player Pointer From the Actor
	aActor->SetPlayer(NULL);
	
	ClientData* cd = player->GetClientData();

	Vector3 position = aActor->GetPosition();
	Vector3 direction = aActor->GetDir();

	//Create a Ghost Actor
	GhostActor* gActor = new GhostActor(player);
	gActor->SetPosition(position);
	gActor->SetDir(direction);

	//Create Ghost behavior
	PlayerGhostBehavior* pGhostBehavior = new PlayerGhostBehavior(gActor, this->zGame->GetWorld(), player);

	this->zGame->SetPlayerBehavior(player, pGhostBehavior);

	//Tell Client his new ID and actor type
	ActorManager* aManager = this->zGame->GetActorManager();
	msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)gActor->GetID());
	msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 2);

	cd->Send(msg);

	//Add the actor to the list
	aManager->AddActor(gActor);
}