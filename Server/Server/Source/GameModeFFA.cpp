#include "Game.h"
#include "time.h"
#include "Physics.h"
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
#include "SupplyDrop.h"
#include "ItemLookup.h"
#include "World/world.h"

GameModeFFA::GameModeFFA( Game* game) : GameMode(game)
{
	srand((unsigned int)time(0));
	this->zSupplyDrop = new SupplyDrop( game->GetActorManager(), game->GetWorld() );
	this->zGameStarted = false;
	this->zGameEnd = false;
	//zKillLimit = killLimit;
}

GameModeFFA::~GameModeFFA()
{
	this->zGame->RemoveObserver(this);

	delete this->zSupplyDrop;
	this->zSupplyDrop = NULL;
}

bool GameModeFFA::Update( float dt )
{
	/*
	if( !this->zGameStarted )
		return true;
		*/

	return false;
}

void GameModeFFA::OnEvent( Event* e )
{
	if( BioActorTakeDamageEvent* ATD = dynamic_cast<BioActorTakeDamageEvent*>(e) )
	{
		if( PlayerActor* pActor = dynamic_cast<PlayerActor*>(ATD->zActor) )
		{
			if(pActor->GetHealth() - ATD->zDamage->GetTotal() <= 0)
			{
				NetworkMessageConverter NMC;
				std::string killsMsg = "";
				std::string msg = "";
				Player* player = pActor->GetPlayer();

				if (ATD->zActor == ATD->zDealer)
				{
					killsMsg = "You killed yourself";
					std::string msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);
				}
				else if (PlayerActor* pDealer = dynamic_cast<PlayerActor*>(ATD->zDealer))
				{
					Player* dealer = pDealer->GetPlayer();
					killsMsg = "You were killed by " + dealer->GetPlayerName();
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);

					killsMsg = "You killed " + player->GetPlayerName();
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					dealer->GetClientData()->Send(msg);

				}
				else if (AnimalActor* pDealer =dynamic_cast<AnimalActor*>(ATD->zDealer))
				{
					killsMsg = "You were killed by an Animal";
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);
				}
				else
				{
					killsMsg = "You Were killed by the environment";
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);
				}

				this->OnPlayerHumanDeath(pActor);
			}
			else
			{
				unsigned int ID = ATD->zDealer->GetID();
				float damage = ATD->zDamage->GetTotal();

				NetworkMessageConverter NMC;
				std::string msg = NMC.Convert(MESSAGE_TYPE_ACTOR_TAKE_DAMAGE, (float)ID);
				msg += NMC.Convert(MESSAGE_TYPE_HEALTH, damage);
				Player* player = pActor->GetPlayer();

				if(player)
				{
					ClientData* cd = player->GetClientData();
					cd->Send(msg);
				}
				
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
					this->zGame->GetActorManager()->RemoveBehavior(aActor);
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
	else if(PlayerAnimalPossessEvent* POSSESSE = dynamic_cast<PlayerAnimalPossessEvent*>(e))
	{

		GhostActor* gActor = dynamic_cast<GhostActor*>(POSSESSE->zActor);

		if(gActor)
			this->PossessAnAnimal(gActor);
				
	}
	else if( PlayerAddEvent* PAE = dynamic_cast<PlayerAddEvent*>(e) )
	{
		zPlayers.insert(PAE->player);
	}
	else if( PlayerRemoveEvent* PRE = dynamic_cast<PlayerRemoveEvent*>(e) )
	{
		auto playerBehavior = PRE->player->GetBehavior();
		if (playerBehavior)
		{
			Actor* actor = playerBehavior->GetActor();

			this->zGame->GetActorManager()->RemoveActor(actor);
		}

		this->zPlayers.erase(PRE->player);
	}
	else if( PlayerReadyEvent* PLRE = dynamic_cast<PlayerReadyEvent*>(e) )
	{
		if(!zGameStarted)
		{
			PLRE->player->SetReady(true);
			//Check how many players are ready
			unsigned int counter = 0;
			for( auto it = zPlayers.begin(); it != zPlayers.end(); it++)
			{
				if( (*it)->IsReady() )
					counter++;
			}

			if( counter >= this->zPlayers.size() )
			{
				this->zGame->RestartGame();
				StartGameMode();
			}
		}
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
	
	if(animalType == 0 && gActor->GetEnergy() < 50)
	{
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not_enough_energy_for_this_animal"));
		gActor->SetEnergy(50);
		return;
	}
	else if(animalType == 2 && gActor->GetEnergy() < 200)
	{
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not_enough_energy_for_this_animal"));
		return;
	}

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
			gActor->SetEnergy( gActor->GetEnergy() - 50);
			closestAnimal->SetEnergy(gActor->GetEnergy());

			animalBehavior = new PlayerDeerBehavior(closestAnimal, this->zGame->GetWorld(), player);

			gActor->SetPlayer(NULL);

			closestAnimal->SetPlayer(player);

			this->zGame->GetActorManager()->RemoveBehavior(closestAnimal);
			this->zGame->SetPlayerBehavior(player, animalBehavior);

			msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)closestAnimal->GetID());
			msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

			cd->Send(msg);

			this->zGame->GetActorManager()->RemoveActor(gActor);
		}
		//Bear
		else if (animalType == 2)
		{
			gActor->SetEnergy( gActor->GetEnergy() - 200);
			closestAnimal->SetEnergy(gActor->GetEnergy());

			animalBehavior = new PlayerBearBehavior(closestAnimal, this->zGame->GetWorld(), player);

			gActor->SetPlayer(NULL);

			closestAnimal->SetPlayer(player);

			this->zGame->GetActorManager()->RemoveBehavior(closestAnimal);

			this->zGame->SetPlayerBehavior(player, animalBehavior);

			msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)closestAnimal->GetID());
			msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

			cd->Send(msg);

			this->zGame->GetActorManager()->RemoveActor(gActor);
		}
	}
	else
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "No animal of the type is close by"));

}

void GameModeFFA::PossessAnAnimal(GhostActor* gActor)
{
	Player* player = gActor->GetPlayer();
	ClientData* cd = player->GetClientData();

	NetworkMessageConverter NMC;
	std::string msg = "";
	AnimalActor* chosenAnimal = NULL;
	float distance = 999999.9f;

	Vector3 position = gActor->GetPosition();

	bool foundAnimal = false;
	int animalType = -1;

	std::set<Actor*> actors;

	auto target = actors.begin();

	this->zGame->GetActorManager()->GetActorsInCircle(position.GetXZ(), 4.0f, actors, ACTOR_TYPE_ANIMAL);
	for (auto it_Actors = actors.begin(); it_Actors != actors.end(); it_Actors++)
	{
		if(GetPhysics()->GetCollisionRayMeshBoundingOnly(gActor->GetPosition(),gActor->GetDir(), (*it_Actors)->GetPhysicsObject()).collision)
		{
			foundAnimal = true;
			target = it_Actors;
		}
	}
	
	if(foundAnimal)
	{
		if (AnimalActor* aActor = dynamic_cast<AnimalActor*>((*target)))
		{
			//If type = Deer
			if (DeerActor* dActor = dynamic_cast<DeerActor*>(aActor))
			{
				if (dActor->IsAlive())
				{
					animalType = 0;
					chosenAnimal = dActor;
				}
				else
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "You are not looking at something alive"));	
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
			if (BearActor* bActor = dynamic_cast<BearActor*>(aActor))
			{
				if (bActor->IsAlive())
				{
					animalType = 2;
					chosenAnimal = bActor;
				}
				else
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "You are not looking at something alive"));
				}
			}
		}

		PlayerBehavior* animalBehavior = NULL;
		if (chosenAnimal)
		{
			//Deer
			if (animalType == 0)
			{

				if(gActor->GetEnergy() < 50 / 2)
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not_enough_energy_for_this_animal"));
					return;
				}
				gActor->SetEnergy( gActor->GetEnergy() - 50 / 2);

				chosenAnimal->SetEnergy(gActor->GetEnergy());

				animalBehavior = new PlayerDeerBehavior(chosenAnimal, this->zGame->GetWorld(), player);

				gActor->SetPlayer(NULL);

				chosenAnimal->SetPlayer(player);

				this->zGame->GetActorManager()->RemoveBehavior(chosenAnimal);
				this->zGame->SetPlayerBehavior(player, animalBehavior);

				msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)chosenAnimal->GetID());
				msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

				cd->Send(msg);

				this->zGame->GetActorManager()->RemoveActor(gActor);
			}
			//Bear
			else if (animalType == 2)
			{

				if(gActor->GetEnergy() < 200 / 2)
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not_enough_energy_for_this_animal"));
					return;
				}
				gActor->SetEnergy( gActor->GetEnergy() - 200 / 2);
				chosenAnimal->SetEnergy(gActor->GetEnergy());

				animalBehavior = new PlayerBearBehavior(chosenAnimal, this->zGame->GetWorld(), player);

				gActor->SetPlayer(NULL);

				chosenAnimal->SetPlayer(player);

				this->zGame->GetActorManager()->RemoveBehavior(chosenAnimal);

				this->zGame->SetPlayerBehavior(player, animalBehavior);

				msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)chosenAnimal->GetID());
				msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 3);

				cd->Send(msg);

				this->zGame->GetActorManager()->RemoveActor(gActor);
			}
		}


	}
	else
	{
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Nothing to possess"));
	}

}

void GameModeFFA::OnPlayerHumanDeath(PlayerActor* pActor)
{
	NetworkMessageConverter NMC;
	std::string msg = "";

	Player* player = pActor->GetPlayer();
	pActor->SetState(STATE_IDLE);
	player->GetKeys().ClearStates();
	//Remove Player Pointer From the Actor
	pActor->SetPlayer(NULL);
	this->zGame->ModifyLivingPlayers(-1);

	ClientData* cd = player->GetClientData();

	//Create Spirit
	Vector3 position = pActor->GetPosition();

	Vector3 direction = pActor->GetDir();
	float energy = pActor->GetEnergy();
	GhostActor* gActor = new GhostActor(player);
	gActor->SetPosition(position);
	gActor->SetDir(direction);
	gActor->SetEnergy(energy + 25.0f);
	gActor->AddObserver(this);
	gActor->SetModel("Media/Models/ghost.obj");
	gActor->SetScale(Vector3());

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

	this->zGame->ModifyLivingPlayers(1);
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

	gActor->SetEnergy(aActor->GetEnergy());

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

bool GameModeFFA::StartGameMode()
{
	//Create Game 

	if( zGameStarted )
		return false;

	this->zGameStarted = true;

	std::set<Item*> items = GenerateItems();
	//this->zSupplyDrop->SpawnSupplyDrop(this->zGame->GetWorld()->GetWorldCenter(), items);
	this->zSupplyDrop->SpawnAirbornSupplyDrop(this->zGame->GetWorld()->GetWorldCenter(), 200.0f, items);

	return true;
}

bool GameModeFFA::SpawnRandomDrop()
{
	return false;
}

std::set<Item*> GameModeFFA::GenerateItems()
{
	/*Just a test*/
	std::set<Item*> items;

	const Food*			temp_food		= GetItemLookup()->GetFood(ITEM_SUB_TYPE_DEER_FOOD);
	const RangedWeapon* temp_R_weapon	= GetItemLookup()->GetRangedWeapon(ITEM_SUB_TYPE_BOW);
	const Projectile*	temp_Arrow		= GetItemLookup()->GetProjectile(ITEM_SUB_TYPE_ARROW);
	const MeleeWeapon*	temp_M_weapon	= GetItemLookup()->GetMeleeWeapon(ITEM_SUB_TYPE_MACHETE);

	Food* food = new Food(*temp_food);
	RangedWeapon* ranged = new RangedWeapon(*temp_R_weapon);
	Projectile* arrow = new Projectile(*temp_Arrow);
	MeleeWeapon* melee = new MeleeWeapon(*temp_M_weapon);

	items.insert(food);
	items.insert(ranged);
	items.insert(arrow);
	items.insert(melee);

	return items;
}
