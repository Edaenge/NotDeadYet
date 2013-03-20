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
#include "Packets/PhysicalConditionPacket.h"
#include "PlayerHumanBehavior.h"

static const unsigned int WEAPON_MIN	= 1;
static const unsigned int WEAPON_MAX	= 2;

static const unsigned int MISC_MAX		= 3;
static const unsigned int MISC_MIN		= 2;

static const unsigned int MATERIAL_MAX	= 4;
static const unsigned int MATERIAL_MIN	= 3;

static const unsigned int VAR			= 5;

//Spawn Random Drop every 10 min
//static const float SPAWN_DROP_TIMER_MAX	= 600.0f;
static const float SPAWN_DROP_TIMER_MAX	= 600.0f;

static const unsigned int NR_PLAYERS_ALIVE_GAME_END_CONDITION = 1;

GameModeFFA::GameModeFFA( Game* game) : GameMode(game)
{
	srand((unsigned int)time(0));
	this->zSupplyDrop = new SupplyDrop( game->GetActorManager(), game->GetWorld(), game->GetSoundHandler() );
	this->zCurrentRSPTime = SPAWN_DROP_TIMER_MAX;
	this->zAlivePlayers = 0;
}

GameModeFFA::~GameModeFFA()
{
	this->zGame->RemoveObserver(this);

	delete this->zSupplyDrop;
	this->zSupplyDrop = NULL;
}

bool GameModeFFA::Update( float dt )
{
	if(!this->zGameStarted)
		return false;

	if( this->zGameEnd )
		return true;

	//Spawn random supply
	this->zCurrentRSPTime -= dt;
	if( zCurrentRSPTime <= 0.0f )
	{
		if( this->SpawnRandomDrop() )
			this->zCurrentRSPTime = SPAWN_DROP_TIMER_MAX;
		else
			this->zCurrentRSPTime = SPAWN_DROP_TIMER_MAX * 0.5f;
	}

	if( CheckEndCondition() )
		return true;

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
					
					if( player && dealer )
					{
						killsMsg = "You were killed by " + dealer->GetPlayerName();
						msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
						player->GetClientData()->Send(msg);
					
						killsMsg = "You killed " + player->GetPlayerName();
						msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
						dealer->GetClientData()->Send(msg);
					}

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

			}
			else
			{
				unsigned int ID = ATD->zDealer->GetID();
				float damage = ATD->zDamage->GetTotal();

				NetworkMessageConverter NMC;
				std::string msg = NMC.Convert(MESSAGE_TYPE_ACTOR_TAKE_DAMAGE, (float)ID);
				msg += NMC.Convert(MESSAGE_TYPE_HEALTH, damage);
				Player* player = pActor->GetPlayer();
				if (player)
				{
					ClientData* cd = player->GetClientData();

					if (cd)
						cd->Send(msg);
				}
				if (ATD->zActor != ATD->zDealer)
				{
					BioActor* bActor = dynamic_cast<BioActor*>(ATD->zDealer);
					if (bActor)
					{
						player = bActor->GetPlayer();
						if(player)
						{
							ClientData* cd = player->GetClientData();
							if (cd)
							{
								std::stringstream ss;
								ss << "You did " << damage << " damage to your target";
								cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, ss.str()));
							}
						}
					}
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
					this->zGame->GetActorManager()->RemoveBehavior(aActor, false);
				}
			}
		}
	}
	else if (BioActorDeathEvent* BADE = dynamic_cast<BioActorDeathEvent*>(e))
	{
		if( !BADE->zActor )
			return;

		PlayerActor* pActor = dynamic_cast<PlayerActor*>(BADE->zActor);

		if( zGameStarted )
		{
			if( pActor )
			{
				Player* player = pActor->GetPlayer();
				if( player )
				{
					//Iterate and count alive players
					this->zAlivePlayers = 0;
					auto it_end = zPlayers.end();
					for(auto it = zPlayers.begin(); it != it_end; it++)
					{
						Behavior* behavior = (*it)->GetBehavior();
						if( behavior )
						{
							BioActor* bActor = dynamic_cast<BioActor*>(behavior->GetActor());
							if( bActor && bActor->IsAlive() )
							{
								this->zAlivePlayers++;
							}
						}
					}
				}

			}
		}

		if( pActor )
		{
			this->OnPlayerHumanDeath(pActor);
		}

		auto models = this->zGame->GetDeadActorModels();

		auto models_it = models.find(BADE->zActor->GetModel());
		if (models_it != models.end())
		{			
			BADE->zActor->SetModel(models_it->second);
		}

		zDeadActors.push_back(BADE->zActor);
		
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
	else if(PlayerGhostMakesNoiseEvent* PGMNE = dynamic_cast<PlayerGhostMakesNoiseEvent*>(e))
	{
		//GhostActor* gActor = dynamic_cast<GhostActor*>(PGMNE->zActor);
	}
	else if( PlayerAddEvent* PAE = dynamic_cast<PlayerAddEvent*>(e) )
	{
		zPlayers.insert(PAE->player);
	}
	else if( PlayerRemoveEvent* PRE = dynamic_cast<PlayerRemoveEvent*>(e) )
	{
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

			if( counter >= this->zPlayers.size() / 2)
			{
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
					if (dActor->IsAlive() && dActor->GetPlayer() == NULL)
					{
						Vector2 center = this->zGame->GetWorld()->GetWorldCenter();
						float radiusFromCenter = (Vector3(center.x, 0.0f, center.y) - dActor->GetPosition()).GetLength();
						if(radiusFromCenter < this->zGame->GetFogEnclosement())
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
					if (bActor->IsAlive() && bActor->GetPlayer() == NULL)
					{

						Vector2 center = this->zGame->GetWorld()->GetWorldCenter();
						float radiusFromCenter = (Vector3(center.x, 0.0f, center.y) - bActor->GetPosition()).GetLength();
						if(radiusFromCenter < this->zGame->GetFogEnclosement())
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

	Vector3 position = gActor->GetPosition();

	bool foundAnimal = false;
	int animalType = -1;

	std::set<Actor*> actors;

	auto target = actors.begin();

	this->zGame->GetActorManager()->GetActorsInCircle(position.GetXZ(), 4.0f, actors, ACTOR_TYPE_ANIMAL);
	for (auto it_Actors = actors.begin(); it_Actors != actors.end(); it_Actors++)
	{
		if(dynamic_cast<BioActor*>((*it_Actors))->IsAlive())
		{
			if(GetPhysics()->GetCollisionRayMeshBoundingOnly(gActor->GetPosition(), gActor->GetDir(), (*it_Actors)->GetPhysicsObject()).collision)
			{
				foundAnimal = true;
				target = it_Actors;
			}
		}
	}
	
	if(foundAnimal)
	{
		if (AnimalActor* aActor = dynamic_cast<AnimalActor*>((*target)))
		{
			//If type = Deer
			if (DeerActor* dActor = dynamic_cast<DeerActor*>(aActor))
			{
				Vector2 center = this->zGame->GetWorld()->GetWorldCenter();
				float radiusFromCenter = (Vector3(center.x, 0.0f, center.y) - dActor->GetPosition()).GetLength();
				
				if (dActor->IsAlive() && dActor->GetPlayer() == NULL && radiusFromCenter < this->zGame->GetFogEnclosement())
				{
					animalType = 0;
					chosenAnimal = dActor;
				}
				else
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "This can not be possessed"));	
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
				Vector2 center = this->zGame->GetWorld()->GetWorldCenter();
				float radiusFromCenter = (Vector3(center.x, 0.0f, center.y) - bActor->GetPosition()).GetLength();


				if (bActor->IsAlive() && bActor->GetPlayer() == NULL && radiusFromCenter < this->zGame->GetFogEnclosement())
				{
					animalType = 2;
					chosenAnimal = bActor;
				}
				else
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "This can not be possessed"));
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
	ActorManager* aManager = this->zGame->GetActorManager();
	Player* player = pActor->GetPlayer();

	if( !player )
		return;

	pActor->SetState(STATE_IDLE);
	player->GetKeys().ClearStates();
	//Remove Player Pointer From the Actor
	pActor->SetPlayer(NULL);
	pActor->GetInventory()->UnEquipAll();
	pActor->GetInventory()->RemoveObserver(player);

	ClientData* cd = player->GetClientData();
	std::map<std::string, std::string> playerModels = zGame->GetPlayerModels();

	Actor* newActor = NULL;
	if( zGameStarted )
	{
		//Create Spirit
		Vector3 position = pActor->GetPosition();
		Vector3 direction = pActor->GetDir();

		float energy = pActor->GetEnergy();

		newActor = new GhostActor(player);
		newActor->SetPosition(position + pActor->GetCameraOffset(), false);
		newActor->SetDir(direction, false);
		newActor->SetEnergy(energy + 25.0f, false);
		newActor->AddObserver(this);
	}
	else
	{
		//Create new Player
		int maxPlayers = zPlayers.size();
		int rand = 1 + (std::rand() % (maxPlayers+1));
		Vector3 position = this->zGame->CalcPlayerSpawnPoint(rand);

		Vector3 direction = pActor->GetDir();
		std::string model = pActor->GetModel();
		std::string objModel = playerModels[model];

// 		if (model.length() > 4)
// 		{
// 			if (model.substr(model.length() - 4) == ".fbx")
// 				model = playerModels[model];
// 		}
		PhysicsObject* pObj = GetPhysics()->CreatePhysicsObject(objModel, position);

		newActor = new PlayerActor(player, pObj, this->zGame);
		newActor->SetEnergy( pActor->GetEnergy() );
		newActor->SetModel(model);
		newActor->SetPosition(position, false);
		newActor->SetDir(direction, false);
		newActor->AddObserver(this);
	}

	PhysicalConditionPacket* PCP = new PhysicalConditionPacket();

	BioActor* bioActor = dynamic_cast<BioActor*>(newActor);

	//Gather Actor Physical Conditions
	PCP->zEnergy = newActor->GetEnergy();

	if( bioActor )
	{
		PCP->zBleedingLevel = bioActor->GetBleeding();
		PCP->zHealth = bioActor->GetHealth();
		PCP->zStamina = bioActor->GetStamina();
		bioActor->SetCameraOffset(zGame->GetOffset( bioActor->GetModel() ));
	}

	PlayerActor* pTemp = dynamic_cast<PlayerActor*>(bioActor);

	if( pTemp )
	{
		PCP->zHunger = pTemp->GetFullness();
		PCP->zHydration = pTemp->GetHydration();
	}

	//Create Ghost behavior
	PlayerBehavior* pBehavior = NULL;
	unsigned int type;
	
	if( zGameStarted )
	{
		pBehavior = new PlayerGhostBehavior(newActor, this->zGame->GetWorld(), player);
		type = 2;
	}
	else
	{
		pBehavior = new PlayerHumanBehavior(newActor, this->zGame->GetWorld(), player);
		type = 1;
	}

	this->zGame->SetPlayerBehavior(player, pBehavior);

	//Tell Client his new ID and actor type
	msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float) newActor->GetID());
	msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, (float)type);

	cd->Send(msg);
	cd->Send(*PCP);
	
	//Add the actor to the list
	aManager->AddActor(newActor);

	delete PCP;
}

void GameModeFFA::OnPlayerAnimalDeath(AnimalActor* aActor)
{
	NetworkMessageConverter NMC;
	std::string msg = "";

	Player* player = aActor->GetPlayer();

	if( !player )
		return;

	//Remove Player Pointer From the Actor
	aActor->SetPlayer(NULL);
	
	ClientData* cd = player->GetClientData();

	Vector3 position = aActor->GetPosition();
	Vector3 direction = aActor->GetDir();

	//Create a Ghost Actor
	GhostActor* gActor = new GhostActor(player);
	gActor->SetPosition(position);
	gActor->SetDir(direction);
	gActor->AddObserver(this);
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
	World* world = this->zGame->GetWorld();
	unsigned int nrOfPlayers = zPlayers.size();
	unsigned int increment = 0; 
	Vector2 center = world->GetWorldCenter();

	//Iterate all players, give them new start positions
	for (auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		Actor* actor = (*it)->GetBehavior()->GetActor();
		
		if( !actor )
		{
			MaloW::Debug("Actor for this: \""+(*it)->GetPlayerName()+"\" player is NULL, in GameModeFFA.");
			continue;
		}

		Vector3 pos = this->zGame->CalcPlayerSpawnPoint(increment++, nrOfPlayers, 10.0f, Vector3(center.x, 0.0f, center.y));

		actor->SetPosition(pos);
		actor->SetEnergy(0);

		if( PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor) )
		{
			pActor->SetHealth( pActor->GetHealthMax() );
			pActor->SetStamina( pActor->GetStaminaMax() );
			pActor->SetBleeding( 0 );
			pActor->SetHydration( pActor->GetHydrationMax() );
			pActor->SetFullness( pActor->GetFullnessMax() );

			//Create And Add pocket knife
			const MeleeWeapon* pocketKnife_temp = GetItemLookup()->GetMeleeWeapon(ITEM_SUB_TYPE_POCKET_KNIFE);
			MeleeWeapon* pocketKnife = new MeleeWeapon(*pocketKnife_temp);

			pActor->GetInventory()->AddItem(pocketKnife);
		}

	}

	//Remove Dead Actors
	ActorManager* aManager = zGame->GetActorManager();
	auto it_end = zDeadActors.end();
	for (auto it = zDeadActors.begin(); it != it_end; it++)
	{
		if( (*it) )
		{
			Actor* temp = *it;
			aManager->RemoveActor(temp);
		}
	}
	zDeadActors.clear();

	this->zSupplyDrop->SpawnSupplyDrop(center, items, 50.0f);

	this->zAlivePlayers = nrOfPlayers;

	NetworkMessageConverter NMC;
	std::string message = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, "Game Started! Good Luck, Stay Safe.");
	this->zGame->SendToAll(message);

	return true;
}

bool GameModeFFA::StopGameMode()
{
	this->zGameEnd = false;
	this->zGameStarted = false;

	for (auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		(*it)->SetReady(false);
	}

	this->zCurrentRSPTime = SPAWN_DROP_TIMER_MAX;
	this->zAlivePlayers = 0;
	this->zDeadActors.clear();


	return true;
}

bool GameModeFFA::SpawnRandomDrop()
{
	World* world = zGame->GetWorld();
	std::set<Item*> items = GenerateItems();
	Vector2 worldCenter = world->GetWorldCenter();

	//Get Fog Enclosure radius from the world center.
	float radius = this->zGame->GetFogEnclosement();

	/***Randomize two players and calculate the pos between them.***/
	std::vector<Actor*> aliveActors;

	//Fetch all alive actors
	for (auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		if( PlayerActor* pActor = dynamic_cast<PlayerActor*>( (*it)->GetBehavior()->GetActor() ) )
		{
			if( pActor->IsAlive() )
				aliveActors.push_back(pActor);
		}
	}

	if( zAlivePlayers != aliveActors.size() )
	{
		MaloW::Debug("zAlivePlayers and aliveActors vector size is no the same: In GameModeFFA, SpawnRandomDrop.");
		zAlivePlayers = aliveActors.size();
	}

	Vector2 size = zGame->GetWorld()->GetWorldSize();
	Vector2 posOne; 
	Vector2 posTwo;
	
	const unsigned int NR_OF_ALIVE_PLAYERS = zAlivePlayers;
	unsigned int playerOne;
	unsigned int playerTwo;

	//Randomize two indicies
	if( NR_OF_ALIVE_PLAYERS > 2 )
	{
		playerOne = rand()% NR_OF_ALIVE_PLAYERS;
		playerTwo = rand()% NR_OF_ALIVE_PLAYERS;
		bool stop = false;
		const unsigned int TRIES = 50;
		unsigned int counter = 0;

		//Make sure the values are Good
		while(!stop && counter < TRIES)
		{
			counter++;

			if(playerOne == playerTwo)
			{
				playerTwo = rand()% NR_OF_ALIVE_PLAYERS;
				continue;
			}

			posOne = aliveActors[playerOne]->GetPosition().GetXZ();
			posTwo = aliveActors[playerTwo]->GetPosition().GetXZ();

			//if the pos is not within the world size given
			if( posOne.x > size.x || posOne.y > size.y )
			{
				playerOne = rand()% NR_OF_ALIVE_PLAYERS;
				continue;
			}
			
			//if the pos is not within the world size given
			if( posTwo.x > size.x || posTwo.y == size.y )
			{
				playerTwo = rand()% NR_OF_ALIVE_PLAYERS;
				continue;
			}

			stop = true;
		}

		//If stop is still false, just use the center points
		if( !stop )
		{
			posOne = worldCenter;
			posTwo = worldCenter;
		}
	}
	//if only two, use them
	else if( NR_OF_ALIVE_PLAYERS == 2 )
	{
		posOne = aliveActors[0]->GetPosition().GetXZ();
		posTwo = aliveActors[1]->GetPosition().GetXZ();

		//if the pos is not within the world size given
		if( posOne.x > size.x || posOne.y > size.y )
		{
			posOne = worldCenter;
		}

		//if the pos is not within the world size given
		if( posTwo.x > size.x || posTwo.y == size.y )
		{
			posTwo = worldCenter;
		}
	}
	//if only one, use it and the world center pos
	else if(NR_OF_ALIVE_PLAYERS == 1)
	{
		posOne = aliveActors[0]->GetPosition().GetXZ();
		posTwo = worldCenter;

		//if the pos is not within the world size given
		if( posOne.x > size.x || posOne.y > size.y )
		{
			posOne = worldCenter;
		}
	}
	//No players?
	else
	{
		return false;
	}

	Vector2 spawnPos;
	Vector2 dir;

	spawnPos = ( (posOne - posTwo) * 0.5f );
	spawnPos = posTwo + spawnPos;
	dir = worldCenter - spawnPos;

	float length = dir.GetLength();
	dir.Normalize();

	//If the pos is within fog enclosure, move it
	if( length > radius )
	{
		float value = length - radius;
		spawnPos += (dir * value);
	}

	//If not inside, something is wrong. World Size is not correct.
	if( !world->IsInside(spawnPos) )
	{
		MaloW::Debug("SupplyDrop is not inside ???");
		return false;
	}

	bool validLocation = false;

	const unsigned int TRIES = 50;
	unsigned int counter = 0;

	//Check if pos is valid, not blocking, not in water
	while( !validLocation && TRIES > counter)
	{
		if( world->IsBlockingAt(spawnPos) )
			spawnPos += dir * 1.0f;
		
		else if( world->GetWaterDepthAt(spawnPos) != 0.0f )
			spawnPos += dir * 2.0f;

		else
			validLocation = true;

		counter++;
	}

	//If not valid after checking x times, give up
	if( !validLocation )
		return false;

	this->zSupplyDrop->SpawnAirbornSupplyDrop(spawnPos, 200.0f, items);

	return true;
}

std::set<Item*> GameModeFFA::GenerateItems()
{

	/** This is not the complete code, this is a test code. **/
	//1-2 * nrOfPlayers/var

	//Randomize Weapon MAX/MIN
	unsigned int weapons	= rand() % WEAPON_MAX + WEAPON_MIN;
	unsigned int misc		= rand() % MISC_MAX + MISC_MIN;
	unsigned int materials	= rand() % MATERIAL_MAX + MATERIAL_MIN;

	unsigned int size = zPlayers.size();
	weapons		*= size / VAR;
	misc		*= size / VAR;
	materials	*= size / VAR;

	if(weapons == 0)
		weapons = WEAPON_MIN;
	if(misc == 0)
		misc = MISC_MIN;
	if(materials == 0)
		materials = MATERIAL_MIN;

	std::set<Item*> items;
	
	//Randomize between Weapons : (2)
	for(unsigned int i = 0; i < weapons; i++)
	{
		Item* item = NULL;
		unsigned int nr = rand() % 2;
		
		if( nr == ITEM_SUB_TYPE_BOW )
			item = new RangedWeapon( *GetItemLookup()->GetRangedWeapon(ITEM_SUB_TYPE_BOW) );

		else if( nr == ITEM_SUB_TYPE_MACHETE)
			item = new MeleeWeapon( *GetItemLookup()->GetMeleeWeapon(ITEM_SUB_TYPE_MACHETE) );

		if( item )
			items.insert(item);
	}
	//Randomize between Material : (5)
	for(unsigned int i = 0; i < materials; i++)
	{
		Item* item = NULL;
		unsigned int nr = rand() % MATERIALS_COUNT;
		
		if( nr == ITEM_SUB_TYPE_SMALL_STICK )
			item = new Material( *GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_SMALL_STICK) );

		else if( nr == ITEM_SUB_TYPE_MEDIUM_STICK )
			item = new Material( *GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_MEDIUM_STICK) );

		else if( nr == ITEM_SUB_TYPE_LARGE_STICK )
			item = new Material( *GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_LARGE_STICK) );

		else if( nr == ITEM_SUB_TYPE_THREAD )
			item = new Material( *GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_THREAD) );

		else if( nr == ITEM_SUB_TYPE_DISENFECTANT_LEAF )
			item = new Material( *GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_DISENFECTANT_LEAF) );

		if( item )
			items.insert(item);
	}

	//Randomize Misc, for now, canteen, food and bandage
	for (unsigned int i = 0; i < misc; i++)
	{
		Item* item = NULL;
		unsigned int nr = rand() % 3;

		if( nr == 0 )
		{
			item = new Food( *GetItemLookup()->GetFood(ITEM_SUB_TYPE_DEER_FOOD) );
		}
		else if ( nr == 1 )
		{
			item = new Container( * GetItemLookup()->GetContainer(ITEM_SUB_TYPE_CANTEEN) );
		}
		else if ( nr == 2 )
		{
			nr = rand()%100 + 1;
			
			if( nr < 80)
			{
				item = new Bandage( *GetItemLookup()->GetBandage(ITEM_SUB_TYPE_BANDAGE_POOR) );
			}
			else
			{
				item = new Bandage( *GetItemLookup()->GetBandage(ITEM_SUB_TYPE_BANDAGE_GREAT) );
			}
		}

		if(item)
		{
			items.insert(item);
		}
	}

	return items;

}

bool GameModeFFA::CanConnect( ClientData* cd )
{
	NetworkMessageConverter NMC;

	if( zGameStarted )
	{
		cd->Send( NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, "Game is already running.") );
		return false;
	}
	
	return true;
}

bool GameModeFFA::CheckEndCondition()
{
	NetworkMessageConverter NMC;

	if( zAlivePlayers == NR_PLAYERS_ALIVE_GAME_END_CONDITION )
	{
		Player* winner_player = NULL;

		//Find the winner
		for (auto it = zPlayers.begin(); it != zPlayers.end() && !winner_player; it++)
		{
			if( PlayerActor* pActor = dynamic_cast<PlayerActor*>((*it)->GetBehavior()->GetActor()) )
			{
				if( pActor->IsAlive() )
				{
					winner_player = (*it);
				}
			}
			else
			{
				continue;
			}
		}

		this->zGameEnd = true;
		
		std::string message;
		if( winner_player )
			message = NMC.Convert( MESSAGE_TYPE_SERVER_ANNOUNCEMENT, "The Winner Is: "+winner_player->GetPlayerName());
		else
			MaloW::Debug("No winner was found");

		zGame->SendToAll(message);
		return true;
	}

	else if( zPlayers.size() == 0 )
	{
		this->zGameEnd = true;
		return true;
	}
	
	return false;
}