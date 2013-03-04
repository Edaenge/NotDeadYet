#include "Game.h"
#include <time.h>
#include "GameModeFFA.h"
#include "GameModeTest.h"
#include "Behavior.h"
#include <world/World.h>
#include <World/EntityList.h>
#include <World/Entity.h>
#include "GameEvents.h"
#include "PlayerHumanBehavior.h"
#include "ProjectileArrowBehavior.h"
#include "PlayerWolfBehavior.h"
#include "PlayerActor.h"
#include "ProjectileActor.h"
#include "DeerActor.h"
#include "WolfActor.h"
#include "BearActor.h"
#include "GhostActor.h"
#include "ItemActor.h"
#include "PlayerDeerBehavior.h"
#include "PlayerBearBehavior.h"
#include "AIDeerBehavior.h"
#include "AIBearBehavior.h"
#include "WorldActor.h"
#include "ItemActor.h"
#include "Physics.h"
#include "ClientServerMessages.h"
#include "ItemLookup.h"
#include "PlayerGhostBehavior.h"
#include <Packets\NewActorPacket.h>
#include "AnimationFileReader.h"
#include "PlayerConfigReader.h"
#include "CraftingManager.h"

static const float PI = 3.14159265358979323846f;
//Total Degrees for the sun to rotate (160 degrees atm)
static const float TOTAL_SUN_DEGREE_SHIFT = 140 * PI / 180;

//Wait time in Seconds for sun Update. (5s atm)
static const float SUN_UPDATE_DELAY = 0.5f;
//Total Sun Update Time in Seconds (6h atm) 
static const float TOTAL_SUN_UPDATE_TIME = 60.0f * 60.0f * 6.0f;

//Expected playtime
static const float EXPECTED_PLAYTIME = 60.0f * 60.0f * 2.0f;

Game::Game( ActorSynchronizer* syncher, std::string mode, const std::string& worldFile ) :
	zSyncher(syncher)
{
	this->zPhysicsEngine = GetPhysics();	
	this->zCameraOffset["Media/Models/temp_guy_movement_anims.fbx"] = Vector3(0.0f, 1.9f, 0.0f);	this->zCameraOffset["Media/Models/temp_guy.obj"] = Vector3(0.0f, 1.9f, 0.0f);
	this->zCameraOffset["Media/Models/deer_temp.obj"] = Vector3(0.0f, 1.7f, 0.0f);
	this->zCameraOffset["Media/Models/Ball.obj"] = Vector3(0.0f, 0.0f, 0.0f);
	
// Create World
	if(worldFile != "")
		this->zWorld = new World(this, worldFile.c_str());
	else
		this->zWorld = new World(this, 10, 10);  // Handle Error.

//Create Crafting Manager
	this->zCraftingManager = new CraftingManager();

// Load Entities
	LoadEntList("Entities.txt");

// Actor Manager
	this->zActorManager = new ActorManager(syncher);
	
	InitItemLookup();

//Initialize Player Configuration file
	InitPlayerConfig();

	this->zMaxNrOfPlayers = 32;
	
//Create GameMode
	if (mode.find("FFA") == 0 )
	{
		this->zGameMode = new GameModeFFA(this);
	}
	else if (mode.find("TestMode") == 0)
	{
		this->zGameMode = new GameModeTest(this, 10);	
	}
	else
	{
		this->zGameMode = new GameModeFFA(this);
	}
// Game Mode Observes
	this->AddObserver(this->zGameMode);

//DEBUG;
	this->SpawnItemsDebug();
	//this->SpawnAnimalsDebug();
	this->SpawnHumanDebug();

//Initialize Sun Direction
	Vector2 mapCenter2D = this->zWorld->GetWorldCenter();

	float radius = mapCenter2D.x;
	float angle = TOTAL_SUN_DEGREE_SHIFT * 0.5f;
	float x = mapCenter2D.x + radius * sin(angle);

	this->zMapCenter = Vector3(mapCenter2D.x, 0.0f, mapCenter2D.y);
	this->zCurrentSunPosition = Vector3(x, 10000.0f, 0.0f);

	this->zCurrentSunDirection =  zMapCenter - this->zCurrentSunPosition;
	this->zCurrentSunDirection.Normalize();

	this->zSunTimer = 0.0f;

	this->zTotalSunRadiansShift = 0.0f;
	this->zSunRadiansShiftPerUpdate = TOTAL_SUN_DEGREE_SHIFT / (SUN_UPDATE_DELAY * TOTAL_SUN_UPDATE_TIME);

//Fog Enclosement
	this->zPlayersAlive = 0;

	Vector2 worldSize = this->zWorld->GetWorldSize();

	radius = (worldSize.x + worldSize.y) * 0.5f;

	this->zInitalFogEnclosement = radius * 0.2f;
	this->zIncrementFogEnclosement = (radius * 2) / this->zMaxNrOfPlayers;

	this->zFogUpdateDelay = 60.0f;
	this->zFogDecreaseCoeff = this->zFogUpdateDelay / EXPECTED_PLAYTIME;
	this->zFogTotalDecreaseCoeff = 1.0f;
	this->zFogTimer = 0.0f;

	this->zCurrentFogEnclosement = ( this->zInitalFogEnclosement + (this->zIncrementFogEnclosement * this->zPlayersAlive) ) * this->zFogTotalDecreaseCoeff;
}

Game::~Game()
{	
	for( auto i = this->zBehaviors.begin(); i != this->zBehaviors.end(); ++i )
	{
		Behavior* data = (*i);
		SAFE_DELETE(data);
	}
	this->zBehaviors.clear();

	for( auto i = this->zPlayers.begin(); i != this->zPlayers.end(); ++i )
	{
		Player* data = i->second;
		if (data)
		{
			delete data;
			data = NULL;
		}
	}
	this->zPlayers.clear();

	SAFE_DELETE(this->zWorld);
	SAFE_DELETE(this->zActorManager);
	SAFE_DELETE(this->zGameMode);

	FreeItemLookup();
	FreePlayerConfig();
}

void Game::SpawnAnimalsDebug()
{
	srand((unsigned int)time(0));
	int increment = 10;
	Vector3 position = this->CalcPlayerSpawnPoint(increment++);
	PhysicsObject* deerPhysics = GetPhysics()->CreatePhysicsObject("Media/Models/deer_temp.obj");
	DeerActor* dActor = new DeerActor(deerPhysics);
	dActor->AddObserver(this->zGameMode);

	Vector3 position2 = this->CalcPlayerSpawnPoint(increment++);
	PhysicsObject* bearPhysics = GetPhysics()->CreatePhysicsObject("Media/Models/deer_temp.obj");
	BearActor* bActor = new BearActor(bearPhysics);
	bActor->AddObserver(this->zGameMode);

	AIDeerBehavior* aiDeerBehavior = new AIDeerBehavior(dActor, this->zWorld);
	AIBearBehavior* aiBearBehavior = new AIBearBehavior(bActor, this->zWorld);

	zBehaviors.insert(aiDeerBehavior);
	zBehaviors.insert(aiBearBehavior);

	dActor->SetPosition(position);
	dActor->SetScale(Vector3(0.05f, 0.05f, 0.05f));

	bActor->SetPosition(position2);
	bActor->SetScale(Vector3(0.08f, 0.08f, 0.08f));

	const Food* temp_Bear_food = GetItemLookup()->GetFood(ITEM_SUB_TYPE_WOLF_FOOD);
	
	int lootSize = (rand() % 5) + 1;
	Food* new_Food = NULL;

	Inventory* inv = bActor->GetInventory();
	bool stacked = false;
	if (temp_Bear_food)
	{
		for (int i = 0; i < lootSize; i++)
		{
			new_Food = new Food((*temp_Bear_food));

			inv->AddItem(new_Food, stacked);
			if( stacked && new_Food->GetStackSize() == 0 )
				SAFE_DELETE(new_Food);
		}
	}

	const Food* temp_Deer_Food = GetItemLookup()->GetFood(ITEM_SUB_TYPE_DEER_FOOD);

	lootSize = (rand() % 7) + 1;
	new_Food = NULL;
	inv = dActor->GetInventory();
	stacked = false;
	if (temp_Deer_Food)
	{
		for (int i = 0; i < lootSize; i++)
		{
			new_Food = new Food((*temp_Deer_Food));

			inv->AddItem(new_Food, stacked);
			if( stacked && new_Food->GetStackSize() == 0 )
				SAFE_DELETE(new_Food);
		}
	}
	
	this->zActorManager->AddActor(dActor);
	this->zActorManager->AddActor(bActor);
}

void Game::SpawnItemsDebug()
{
	//ITEMS
	const Food*			temp_food		= GetItemLookup()->GetFood(ITEM_SUB_TYPE_DEER_FOOD);
	const RangedWeapon* temp_R_weapon	= GetItemLookup()->GetRangedWeapon(ITEM_SUB_TYPE_BOW);
	const Projectile*	temp_Arrow		= GetItemLookup()->GetProjectile(ITEM_SUB_TYPE_ARROW);
	const MeleeWeapon*	temp_M_weapon	= GetItemLookup()->GetMeleeWeapon(ITEM_SUB_TYPE_MACHETE);
	const Material*		temp_material_S	= GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_SMALL_STICK);
	const Material*		temp_material_M	= GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_MEDIUM_STICK);
	const Material*		temp_material_T	= GetItemLookup()->GetMaterial(ITEM_SUB_TYPE_THREAD);
	const Bandage*		temp_bandage	= GetItemLookup()->GetBandage(ITEM_SUB_TYPE_BANDAGE_POOR);
	const Bandage*		temp_bandage_G	= GetItemLookup()->GetBandage(ITEM_SUB_TYPE_BANDAGE_GREAT);

	unsigned int increment = 0;
	int maxPoints = 10;
	float radius = 3.5f;
	int numberOfObjects = 8;
	int total = 0;
	Vector3 center;
	Vector3 position;
	Vector2 tempCenter = this->zWorld->GetWorldCenter();
	for (int i = 0; i < maxPoints; i++)
	{
		center = Vector3(tempCenter.x, 0, tempCenter.y);
		int currentPoint = i % maxPoints;

		center = this->CalcPlayerSpawnPoint(currentPoint, maxPoints, 17.0f, center);

		//Food
		if (temp_food)
		{
			Food* new_Item = new Food((*temp_food));
			ItemActor* actor = new ItemActor(new_Item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Weapon_ranged
		if(temp_R_weapon)
		{
			RangedWeapon* new_item = new RangedWeapon((*temp_R_weapon));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Arrows
		if(temp_Arrow)
		{
			Projectile* new_item = new Projectile((*temp_Arrow));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Melee_weapon
		if(temp_M_weapon)
		{
			MeleeWeapon* new_item = new MeleeWeapon((*temp_M_weapon));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Small_stick
		if(temp_material_S)
		{
			Material* new_item = new Material((*temp_material_S));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Medium_stick
		if(temp_material_M)
		{
			Material* new_item = new Material((*temp_material_M));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Thread
		if(temp_material_T)
		{
			Material* new_item = new Material((*temp_material_T));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		//Bandage
		if(temp_bandage)
		{
			Bandage* new_item = new Bandage((*temp_bandage));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		if(temp_bandage_G)
		{
			Bandage* new_item = new Bandage((*temp_bandage_G));
			ItemActor* actor = new ItemActor(new_item);
			//center = CalcPlayerSpawnPoint(increment++);
			position = this->CalcPlayerSpawnPoint(increment++, numberOfObjects, radius, center);
			actor->SetPosition(position);
			actor->SetScale(Vector3(0.05f, 0.05f, 0.05f));
			this->zActorManager->AddActor(actor);
		}
		total += increment;
		increment = 0;
	}
}

void Game::SpawnHumanDebug()
{
	srand((unsigned int)time(0));
	int increment = 10;
	Vector3 position = this->CalcPlayerSpawnPoint(increment++);
	PhysicsObject* humanPhysics = GetPhysics()->CreatePhysicsObject("Media/Models/temp_guy.obj");
	humanPhysics->SetModel("Media/Models/temp_guy_movement_anims.fbx");
	PlayerActor* pActor = new PlayerActor(NULL, humanPhysics);
	pActor->AddObserver(this->zGameMode);
	pActor->SetPosition(position);
	pActor->SetHealth(5000);
	pActor->SetScale(pActor->GetScale());
	this->zActorManager->AddActor(pActor);
}

void Game::UpdateSunDirection(float dt)
{
	//Update Sun
	this->zSunTimer += dt;

	if (this->zSunTimer >= SUN_UPDATE_DELAY)
	{
		Vector2 worldCenter = this->zWorld->GetWorldCenter();
		float radius = worldCenter.x;
		float angle = TOTAL_SUN_DEGREE_SHIFT * 0.5f + this->zTotalSunRadiansShift;
		float x = worldCenter.x + radius * sin(angle);

		this->zTotalSunRadiansShift += this->zSunRadiansShiftPerUpdate;

		if (this->zTotalSunRadiansShift >= TOTAL_SUN_DEGREE_SHIFT)
			this->zTotalSunRadiansShift = 0.0f;

		this->zCurrentSunPosition.x = x;

		this->zCurrentSunDirection = this->zMapCenter - this->zCurrentSunPosition;
		this->zCurrentSunDirection.Normalize();

		NetworkMessageConverter NMC;
		std::string msg = NMC.Convert(MESSAGE_TYPE_SUN_DIRECTION, this->zCurrentSunDirection);

		this->SendToAll(msg);

		this->zSunTimer = 0.0f;
	}
}

void Game::UpdateFogEnclosement( float dt )
{
	this->zFogTimer += dt;

	if (this->zFogTimer >= this->zFogUpdateDelay)
	{
		if (this->zCurrentFogEnclosement > this->zInitalFogEnclosement)
		{
			this->zFogTotalDecreaseCoeff -= this->zFogDecreaseCoeff;

			this->zCurrentFogEnclosement = (this->zInitalFogEnclosement + (this->zIncrementFogEnclosement * this->zPlayersAlive) ) * this->zFogTotalDecreaseCoeff;
			
			NetworkMessageConverter NMC;
			this->SendToAll(NMC.Convert(MESSAGE_TYPE_FOG_ENCLOSEMENT, this->zCurrentFogEnclosement));
		}
		this->zFogTimer = 0.0f;
	}
}

bool Game::Update( float dt )
{
	this->UpdateSunDirection(dt);
	this->UpdateFogEnclosement(dt);
	NetworkMessageConverter NMC;
	std::string msg;

	// Update Behaviors
	auto i = zBehaviors.begin();
	int counter = 0;
	while( i != zBehaviors.end() )
	{
		if(PlayerActor* cActor = dynamic_cast<PlayerActor*>((*i)->GetActor()))
		{
			if(cActor->GetStamina() < 25.0f)
			{
				if(cActor->UpdateBreathSoundTimer(dt))
				{
					NetworkMessageConverter NMC;
					std::string msg = NMC.Convert(MESSAGE_TYPE_PLAY_SOUND, "Media/Sound/Running_Breath_4.mp3");
					msg += NMC.Convert(MESSAGE_TYPE_POSITION, cActor->GetPosition());
					this->SendToAll(msg);
				}
			}
		}

		if( PlayerBehavior* playerBehavior = dynamic_cast<PlayerBehavior*>((*i)) )
		{
			playerBehavior->RefreshNearCollideableActors(zActorManager->GetActors());
		}
		else if( ProjectileArrowBehavior* projectileArrowBehavior = dynamic_cast<ProjectileArrowBehavior*>(*i) )
		{
			projectileArrowBehavior->RefreshNearCollideableActors(zActorManager->GetActors());
		}
		
		if ( (*i)->Update(dt) )
		{
			Behavior* temp = (*i);
			Actor* oldActor = NULL;
			ItemActor* newActor = ConvertToItemActor(temp, oldActor);
			
			i = zBehaviors.erase(i);
			
			delete temp;
			temp = NULL;
			
			this->zPhysicsEngine->DeletePhysicsObject(oldActor->GetPhysicsObject());
			this->zActorManager->RemoveActor(oldActor);
			this->zActorManager->AddActor(newActor);
		}
		else
		{
			i++;
			counter++;
		}
	}

	// Update Game Mode, Might Notify That GameMode is Finished
	if ( this->zGameMode->Update(dt) )
		return false;

	// Update World
	zWorld->Update();

	//Updating animals and Check fog.
	for(i = zBehaviors.begin(); i != zBehaviors.end(); i++)
	{
		if(AIDeerBehavior* animalBehavior = dynamic_cast<AIDeerBehavior*>( (*i) ))
		{
			animalBehavior->SetCurrentTargets(counter);
		}
		else if(AIBearBehavior* animalBehavior = dynamic_cast<AIBearBehavior*>( (*i) ))
		{
			animalBehavior->SetCurrentTargets(counter);
		}
		else if (PlayerBehavior* playerBehavior = dynamic_cast<PlayerBehavior*>( (*i) ))
		{
			if (BioActor* bActor = dynamic_cast<BioActor*>( (*i)->GetActor() ))
			{
				Vector2 center = this->zWorld->GetWorldCenter();

				float radiusFromCenter = (Vector3(center.x, 0.0f, center.y) - bActor->GetPosition()).GetLength();

				if (radiusFromCenter > this->zCurrentFogEnclosement)
				{
					Damage dmg;
					dmg.fogDamage = 1.0f * dt;
					bActor->TakeDamage(dmg, bActor);
				}
			}
		}
	}

	for(i = zBehaviors.begin(); i != zBehaviors.end(); i++)
	{
		int counter = 0;
		for(auto j = zBehaviors.begin(); j != zBehaviors.end(); j++)
		{
			
			if(AIDeerBehavior* animalBehavior = dynamic_cast<AIDeerBehavior*>(*i))
			{

				if(AIDeerBehavior* tempBehaviour = dynamic_cast<AIDeerBehavior*>(*j))
				{
					//tempBehaviour->get_
					//Actor* oldActor = NULL;
					//ItemActor* newActor = ConvertToItemActor(tempBehaviour, oldActor);
					animalBehavior->SetTargetInfo(counter, tempBehaviour->GetActor()->GetPosition(), 1.0f, 100.0f, DEER);
				}
				else if(PlayerHumanBehavior* tempBehaviour = dynamic_cast<PlayerHumanBehavior*>(*j))
				{
					//Actor* oldActor = NULL;
					//ItemActor* newActor = ConvertToItemActor(tempBehaviour, oldActor);
					animalBehavior->SetTargetInfo(counter, tempBehaviour->GetActor()->GetPosition(), 1.0f, 100.0f, HUMAN);
				}
				else if(AIBearBehavior* tempBehaviour = dynamic_cast<AIBearBehavior*>(*j))
				{
					animalBehavior->SetTargetInfo(counter, tempBehaviour->GetActor()->GetPosition(), 1.0f, 100.0f, BEAR);
				}
			}
			else if(AIBearBehavior* animalBehavior = dynamic_cast<AIBearBehavior*>(*i))
			{
				//animalBehavior->SetTargetInfo(counter,(*j)
				if(AIDeerBehavior* tempBehaviour = dynamic_cast<AIDeerBehavior*>(*j))
				{
					//tempBehaviour->get_
					animalBehavior->SetTargetInfo(counter, tempBehaviour->GetActor()->GetPosition(), 1.0f, 100.0f, DEER);
				}
				else if(PlayerHumanBehavior* tempBehaviour = dynamic_cast<PlayerHumanBehavior*>(*j))
				{
					animalBehavior->SetTargetInfo(counter, tempBehaviour->GetActor()->GetPosition(), 1.0f, 100.0f, HUMAN);
				}
				else if(AIBearBehavior* tempBehaviour = dynamic_cast<AIBearBehavior*>(*j))
				{
					//Actor* oldActor = NULL;
					//ItemActor* newActor = ConvertToItemActor(tempBehaviour, oldActor);
					animalBehavior->SetTargetInfo(counter, tempBehaviour->GetActor()->GetPosition(), 1.0f, 100.0f, BEAR);
				}
			}
			counter++;
		}
	}

/*	// Collisions Tests
	for(i = zBehaviors.begin(); i != zBehaviors.end(); i++)
	{
		//*** Projectiles ***
		if(ProjectileArrowBehavior* projBehavior = dynamic_cast<ProjectileArrowBehavior*>(*i))
		{
			ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(projBehavior->GetActor());
			if(!projActor)
			{
				MaloW::Debug("ProjectileActor is null. Arrow collision detection in Game.cpp, Update.");;
				continue;
			}

			//Get Data
			float length = projBehavior->GetLength();
			float distance = length;
			//Check collision, returns the result
			Actor* collide = this->zActorManager->CheckCollisions(projActor, distance); 

			if( BioActor* victim = dynamic_cast<BioActor*>(collide) )
			{
				//Stop arrow
				projBehavior->Stop();
				
				//Take damage
				victim->TakeDamage(projActor->GetDamage(), projActor->GetOwner());
			}
			else if( WorldActor* object = dynamic_cast<WorldActor*>(collide) )
			{
				//Stop Arrow
				projBehavior->Stop();
			}

		}
		//*** Ghosts, ignore ***
		else if( dynamic_cast<PlayerGhostBehavior*>(*i) )
		{
			continue;
		}
		//*** AI, ignore ***
		else if( dynamic_cast<AIBehavior*>(*i) )
		{
			continue;
		}
		//*** Others ***
		else
		{
			BioActor* pActor = dynamic_cast<BioActor*>((*i)->GetActor());

			//If actor hasn't moved, ignore
			if( pActor && !pActor->HasMoved() )
				continue;

			Actor* collide = NULL;
			float range = 1.0f; //hard coded

			collide = this->zActorManager->CheckCollisionsByDistance(pActor, range);

			//No collision, ignore the rest
			if(!collide)
				continue;

			Vector3 pActor_rewind_dir = (collide->GetPosition() - pActor->GetPosition());
			pActor_rewind_dir.Normalize();

			if( BioActor* target = dynamic_cast<BioActor*>(collide) )
			{
				//Calculate Target rewind dir.
				Vector3 target_rewind_dir = pActor_rewind_dir * -1;
	
				//If target did not move, do not rewind position.
				if(target->HasMoved())
					target->SetPosition( target->GetPosition() - (target_rewind_dir * 0.25f) );

				pActor->SetPosition( pActor->GetPosition() - (pActor_rewind_dir * 0.25f) );
			}
			else if( WorldActor* object = dynamic_cast<WorldActor*>(collide) )
			{
				//Rewind the pActor only.
				pActor_rewind_dir = pActor_rewind_dir;
				pActor->SetPosition( pActor->GetPosition() - (pActor_rewind_dir * 0.25f) );
			}
		}
	}
	*/

	// Game Still Active
	return true;
}

void Game::OnEvent( Event* e )
{
	// TODO: Incoming Message

	if ( PlayerConnectedEvent* PCE = dynamic_cast<PlayerConnectedEvent*>(e) )
	{
		HandleConnection(PCE->clientData);
	}
	else if( UserReadyEvent* URE = dynamic_cast<UserReadyEvent*>(e) )
	{
		PlayerReadyEvent playerReady;
		playerReady.player = this->zPlayers[URE->clientData];
		
		NotifyObservers(&playerReady);
	}
	else if( KeyDownEvent* KDE = dynamic_cast<KeyDownEvent*>(e) )
	{
		zPlayers[KDE->clientData]->GetKeys().SetKeyState(KDE->key, true);
	}
	else if( KeyUpEvent* KUE = dynamic_cast<KeyUpEvent*>(e) )
	{
		zPlayers[KUE->clientData]->GetKeys().SetKeyState(KUE->key, false);
	}
	else if ( PlayerSwapEquippedWeaponsEvent* SEQWE = dynamic_cast<PlayerSwapEquippedWeaponsEvent*>(e) )
	{
		Actor* actor = this->zPlayers[SEQWE->clientdData]->GetBehavior()->GetActor();

		if(BioActor* pActor = dynamic_cast<BioActor*>(actor))
		{
			Inventory* inv = pActor->GetInventory();
			if(inv->SwapWeapon())
			{
				NetworkMessageConverter NMC;
				std::string msg;

				Item* item = inv->GetSecondaryEquip();

				msg = NMC.Convert(MESSAGE_TYPE_MESH_UNBIND, (float)pActor->GetID());
				msg += NMC.Convert(MESSAGE_TYPE_MESH_MODEL, item->GetModel());
				SEQWE->clientdData->Send(msg);

				Item* newPrimary = inv->GetPrimaryEquip();

				if (newPrimary)
					this->HandleBindings(newPrimary, pActor->GetID());
			}
		}
	}
	else if( ClientDataEvent* CDE = dynamic_cast<ClientDataEvent*>(e) )
	{
		Player* player = zPlayers[CDE->clientData];
		if (player)
			if( PlayerBehavior* dCastBehavior = dynamic_cast<PlayerBehavior*>(player->GetBehavior()))
				dCastBehavior->ProcessClientData(CDE->direction, CDE->rotation);
	}
	else if ( PlayerDisconnectedEvent* PDCE = dynamic_cast<PlayerDisconnectedEvent*>(e) )
	{
		HandleDisconnect(PDCE->clientData);
	}
	else if(PlayerLootObjectEvent* PLOE = dynamic_cast<PlayerLootObjectEvent*>(e))
	{
		HandleLootObject(PLOE->clientData, PLOE->actorID);
	}
	else if ( PlayerLootItemEvent* PLIE = dynamic_cast<PlayerLootItemEvent*>(e) )
	{
		HandleLootItem(PLIE->clientData, PLIE->itemID, PLIE->itemType, PLIE->objID, PLIE->subType);		
	}
	else if ( PlayerDropItemEvent* PDIE = dynamic_cast<PlayerDropItemEvent*>(e) )
	{
		HandleDropItem(PDIE->clientData, PDIE->itemID);
	}
	else if (PlayerUseItemEvent* PUIE = dynamic_cast<PlayerUseItemEvent*>(e))
	{
		HandleUseItem(PUIE->clientData, PUIE->itemID);
	}
	else if (PlayerCraftItemEvent* PCIE = dynamic_cast<PlayerCraftItemEvent*>(e))
	{
		HandleCraftItem(PCIE->clientData, PCIE->itemID);
	}
	else if ( PlayerUseEquippedWeaponEvent* PUEWE = dynamic_cast<PlayerUseEquippedWeaponEvent*>(e) )
	{
		HandleUseWeapon(PUEWE->clientData, PUEWE->itemID);
	}
	else if(PlayerAnimalAttackEvent* PAAE = dynamic_cast<PlayerAnimalAttackEvent*>(e))
	{
		auto playerIterator = this->zPlayers.find(PAAE->clientData);
		Player* player = playerIterator->second;
		auto it = playerIterator->second->GetBehavior();
		Actor* self = it->GetActor();

		float range = 0.0f;
		Damage damage;
		
		if(BearActor* bActor = dynamic_cast<BearActor*>(self))
		{
			if(player)
			{
				if(PAAE->mouseButton == MOUSE_LEFT_PRESS)
				{
					range = 2.2f;
					damage.blunt = 10.0f;
				}
				else if(PAAE->mouseButton == MOUSE_RIGHT_PRESS)
				{
					range = 1.5f;
					damage.piercing = 5.0f;
					damage.slashing = 25.0f;
				}
				if(Actor* target = this->zActorManager->CheckCollisions(self, range))
				{
					BioActor* bActor = dynamic_cast<BioActor*>(target);
					if(bActor->IsAlive())
					{
						bActor->TakeDamage(damage,self);
					}
				}
			}
		}

	}
	else if (PlayerEquipItemEvent* PEIE = dynamic_cast<PlayerEquipItemEvent*>(e) )
	{
		HandleEquipItem(PEIE->clientData, PEIE->itemID);
	}
	else if (PlayerUnEquipItemEvent* PUEIE = dynamic_cast<PlayerUnEquipItemEvent*>(e) )
	{
		HandleUnEquipItem(PUEIE->clientData, PUEIE->itemID, PUEIE->eq_Slot);
	}
	else if(PlayerAnimalSwapEvent* PASE = dynamic_cast<PlayerAnimalSwapEvent*>(e))
	{
		auto playerIterator = this->zPlayers.find(PASE->clientData);
		Player* player = playerIterator->second;

		if (player)
		{
			Behavior* playerBehavior = player->GetBehavior();
			if (playerBehavior)
			{
				PASE->zActor = playerBehavior->GetActor();
			}
		}
	}
	else if (PlayerLeaveAnimalEvent* PLAE = dynamic_cast<PlayerLeaveAnimalEvent*>(e))
	{
		auto playerIterator = this->zPlayers.find(PLAE->clientData);

		Player* player = playerIterator->second;
		NetworkMessageConverter NMC;
		std::string msg;
		if (player)
		{
			Behavior* playerBehavior = player->GetBehavior();
			if (playerBehavior)
			{
				Actor* actor = playerBehavior->GetActor();

				if(DeerActor* dActor = dynamic_cast<DeerActor*>(actor))
				{
					dActor->SetPlayer(NULL);
					AIDeerBehavior* behavior = new AIDeerBehavior(dActor, this->zWorld);

					this->zBehaviors.insert(behavior);

					//Create Ghost behavior And Ghost Actor
					GhostActor* gActor = new GhostActor(player);
					gActor->SetPosition(dActor->GetPosition());

					PlayerGhostBehavior* playerGhostBehavior = new PlayerGhostBehavior(gActor, this->zWorld, player);

					this->SetPlayerBehavior(player, playerGhostBehavior);

					//Tell Client his new ID and actor type
					msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)gActor->GetID());
					msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 2);
					PLAE->clientData->Send(msg);

					//Add the actor to the list
					this->zActorManager->AddActor(gActor);

					NetworkMessageConverter NMC;
					std::string msg = NMC.Convert(MESSAGE_TYPE_SUN_DIRECTION, this->zCurrentSunDirection);
				}
			}
		}
	}
	else if (PlayerDeerEatObjectEvent* PDEOE = dynamic_cast<PlayerDeerEatObjectEvent*>(e))
	{
		//ID's of the Object deer is trying to eat
		auto playerIterator = this->zPlayers.find(PDEOE->clientData);
		auto playerBehavior = playerIterator->second->GetBehavior();
		Actor* actor = playerBehavior->GetActor();

		NetworkMessageConverter NMC;
		std::string msg = NMC.Convert(MESSAGE_TYPE_LOOT_OBJECT_RESPONSE);
		unsigned int ID = 0;
		bool bEaten = false;

		std::set<Actor*> actors = this->zActorManager->GetActors();

		DeerActor* thePlayerActor;

		ItemActor* toBeRemoved = NULL;

		Damage idiotDamage;

		thePlayerActor = dynamic_cast<DeerActor*>(actor);
		if(thePlayerActor)
		{
			for (auto it_actor = actors.begin(); it_actor != actors.end() && !bEaten; it_actor++)
			{
				//Loop through all ID's of all actors the client tried to loot.
				for (auto it_ID = PDEOE->actorID.begin(); it_ID != PDEOE->actorID.end(); it_ID++)
				{
					//Check if the ID is the same.
					if ((*it_ID) == (*it_actor)->GetID())
					{
						//Check if the distance between the actors are too far to be able to loot.
						if ((actor->GetPosition() - (*it_actor)->GetPosition()).GetLength() > 5.0f)
							continue;

						//Check if the Actor is an ItemActor
						if (ItemActor* iActor = dynamic_cast<ItemActor*>(*it_actor))
						{
							ID = iActor->GetID();
							Item* theItem = iActor->GetItem();
							if(theItem->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
							{
								idiotDamage.piercing = 25.0f;
						
								thePlayerActor->TakeDamage(idiotDamage,actor);
							}
							else if(theItem->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
							{
								Damage idiotDamage;
								idiotDamage.piercing = 10.0f;
						
								thePlayerActor->TakeDamage(idiotDamage,actor);
							}
							else if(theItem->GetItemSubType() == ITEM_SUB_TYPE_ROCK)
							{
								Damage idiotDamage;
								idiotDamage.piercing = 5.0f;
						
								thePlayerActor->TakeDamage(idiotDamage,actor);
							}
							toBeRemoved = iActor;
							bEaten = true;
						}
					}
				}
			}
		}

		if(bEaten)
			this->zActorManager->RemoveActor(toBeRemoved);
	}
	else if ( EntityUpdatedEvent* EUE = dynamic_cast<EntityUpdatedEvent*>(e) )
	{
		//auto i = zWorldActors.find(EUE->entity);
		//if ( i != zWorldActors.end() )
		//{
		//	i->second->SetPosition(EUE->entity->GetPosition());
		//	// TODO: Rotation
		//	i->second->SetScale(EUE->entity->GetScale());
		//}
	}
	else if ( EntityLoadedEvent* ELE = dynamic_cast<EntityLoadedEvent*>(e) )
	{
		PhysicsObject* phys = NULL;
		
		if ( GetEntBlockRadius(ELE->entity->GetType()) > 0.0f )
		{
			// Create Physics Object
			phys = zPhysicsEngine->CreatePhysicsObject(GetEntModel(ELE->entity->GetType()));
		}

		WorldActor* actor = new WorldActor(phys);
		actor->SetPosition(ELE->entity->GetPosition());
		actor->SetScale(actor->GetScale());
		actor->AddObserver(this->zGameMode);
		
		this->zWorldActors[ELE->entity] = actor;
		this->zActorManager->AddActor(actor);
		
	}
	else if ( EntityRemovedEvent* ERE = dynamic_cast<EntityRemovedEvent*>(e) )
	{
		auto i = zWorldActors.find(ERE->entity);
		if ( i != zWorldActors.end() )
		{
			zPhysicsEngine->DeletePhysicsObject(i->second->GetPhysicsObject());
			this->zActorManager->RemoveActor(i->second);
			this->zWorldActors.erase(i);
		}
	}
	else if ( UserDataEvent* UDE = dynamic_cast<UserDataEvent*>(e) )
	{
		// Create Player Actor
		PhysicsObject* pObj = this->zPhysicsEngine->CreatePhysicsObject("Media/Models/temp_guy.obj");
		pObj->SetModel(UDE->playerModel);
		Actor* actor = new PlayerActor(zPlayers[UDE->clientData], pObj);
		zPlayers[UDE->clientData]->zUserName = UDE->playerName;
		zPlayers[UDE->clientData]->zUserModel = UDE->playerModel;

		
		actor->AddObserver(this->zGameMode);
		Vector3 center;

		// Start Position
		center = this->CalcPlayerSpawnPoint(32, zWorld->GetWorldCenter());
		actor->SetPosition(center);
		actor->SetScale(actor->GetScale());
		
		zActorManager->AddActor(actor);

		auto offsets = this->zCameraOffset.find(UDE->playerModel);
		
		if(offsets != this->zCameraOffset.end())
			dynamic_cast<PlayerActor*>(actor)->SetCameraOffset(offsets->second);

		// Apply Default Player Behavior
		SetPlayerBehavior(zPlayers[UDE->clientData], new PlayerHumanBehavior(actor, zWorld, zPlayers[UDE->clientData]));

		//Tells the client which Actor he owns.
		std::string message;
		NetworkMessageConverter NMC;
		unsigned int selfID;

		selfID = actor->GetID();
		message = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)selfID);
		message += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, (float)1);

		UDE->clientData->Send(message);

		NewActorPacket* NAP = new NewActorPacket();

		//Gather Actors Information and send to client
		std::set<Actor*>& actors = this->zActorManager->GetActors();
		for (auto it = actors.begin(); it != actors.end(); it++)
		{
			if(actor == (*it))
				continue;

			if(dynamic_cast<WorldActor*>(*it))
				continue;

			NAP->actorPosition[(*it)->GetID()] = (*it)->GetPosition();
			NAP->actorRotation[(*it)->GetID()] = (*it)->GetRotation();
			NAP->actorScale[(*it)->GetID()] = (*it)->GetScale();
			NAP->actorModel[(*it)->GetID()] = (*it)->GetModel();

			if (BioActor* bActor = dynamic_cast<BioActor*>( (*it) ))
				NAP->actorState[bActor->GetID()] = bActor->GetState();
		}
		UDE->clientData->Send(*NAP);
		SAFE_DELETE(NAP);

		this->zPlayersAlive++;
		this->zCurrentFogEnclosement = this->zInitalFogEnclosement + ( this->zIncrementFogEnclosement * this->zPlayersAlive);

		message = NMC.Convert(MESSAGE_TYPE_FOG_ENCLOSEMENT, this->zCurrentFogEnclosement);
		this->SendToAll(message);
	}
	else if ( WorldLoadedEvent* WLE = dynamic_cast<WorldLoadedEvent*>(e) )
	{

	}
	else if ( PlayerKillEvent* PKE = dynamic_cast<PlayerKillEvent*>(e) )
	{
		ClientData* cd = PKE->clientData;

		Actor* actor = this->zPlayers[cd]->GetBehavior()->GetActor();

		BioActor* bActor = dynamic_cast<BioActor*>(actor);

		if (bActor)
			bActor->Kill();
	}

	NotifyObservers(e);
}

void Game::SetPlayerBehavior( Player* player, PlayerBehavior* behavior )
{
	// Find Old Behavior
	Behavior* curPlayerBehavior = player->GetBehavior();

	// Find In Behaviors
	if ( curPlayerBehavior )
	{
		zBehaviors.erase(curPlayerBehavior);
		delete curPlayerBehavior;
		curPlayerBehavior = NULL;
	}

	// Set New Behavior
	if ( behavior )	
	{
		zBehaviors.insert(behavior);
		std::set<Actor*> actors;
		this->zActorManager->GetCollideableActorsInCircle(behavior->GetActor()->GetPosition().GetXZ(), behavior->GetCollisionRadius(), actors);
		behavior->SetNearActors(actors);
	}

	player->zBehavior = behavior;
}

void Game::RemoveAIBehavior( AnimalActor* aActor )
{
	for (auto it_behavior = this->zBehaviors.begin(); it_behavior != this->zBehaviors.end();)
	{
		if ((*it_behavior)->GetActor() == aActor)
		{
			it_behavior = this->zBehaviors.erase(it_behavior);
		}
		else
		{
			it_behavior++;
		}
	}
}

Vector3 Game::CalcPlayerSpawnPoint(int currentPoint, int maxPoints, float radius, Vector3 center)
{
	float slice  = 2 * PI / maxPoints;

	float angle = slice * currentPoint;

	float x = center.x + radius * cos(angle);
	float z = center.z + radius * sin(angle);
	float y = 0.0f;

	if ( x >= 0.0f && y >= 0.0f && x<zWorld->GetWorldSize().x && y<zWorld->GetWorldSize().y )
	{
		y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));
	}

	return Vector3(x, y, z);
}

Vector3 Game::CalcPlayerSpawnPoint(int maxPoints, Vector2 center)
{
	int point = this->zPlayers.size();

	static const float radius = 20.0f;
	float slice  = 2 * PI / maxPoints;

	float angle = slice * point;

	float x = center.x + radius * cos(angle);
	float z = center.y + radius * sin(angle);
	float y = 0.0f;

	if ( x >= 0.0f && z >= 0.0f && x < zWorld->GetWorldSize().x && z < zWorld->GetWorldSize().y )
	{
		y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));
	}

	return Vector3(x, y, z);
}

Vector3 Game::CalcPlayerSpawnPoint(int nr)
{
	int point = nr;

	static const float radius = 20.0f;
	float slice  = 2 * PI / this->zMaxNrOfPlayers;

	float angle = slice * point;

	float x = zWorld->GetWorldCenter().x + radius * cos(angle);
	float z = zWorld->GetWorldCenter().y + radius * sin(angle);
	float y = 0.0f;

	if ( x >= 0.0f && z >= 0.0f && x < zWorld->GetWorldSize().x && z < zWorld->GetWorldSize().y )
	{
		y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));
	}

	return Vector3(x, y, z);
}

ItemActor* Game::ConvertToItemActor(Behavior* behavior, Actor*& oldActorOut)
{
	int itemType = 0;
	ProjectileArrowBehavior* projBehavior = dynamic_cast<ProjectileArrowBehavior*>(behavior);

	//Check what kind of projectile
	if(projBehavior)
		itemType = ITEM_SUB_TYPE_ARROW;
	//else if(false) //Else if stone
	//	itemType = ITEM_SUB_TYPE_ROCK;
	else
		return NULL;
	
	//Fetch the actor
	ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(projBehavior->GetActor());

	if(!projActor)
		return NULL;

	//Get the item based on type
	const Projectile* item = GetItemLookup()->GetProjectile(itemType);

	if(!item)
		return NULL;
	Projectile* projectile = new Projectile((*item));
	projectile->SetStackSize(1);

	ItemActor* itemActor = new ItemActor(projectile);
	itemActor->SetPosition(projActor->GetPosition());
	itemActor->SetRotation(projActor->GetRotation());
	itemActor->SetScale(projActor->GetScale());
	itemActor->SetDir(projActor->GetDir());
	oldActorOut = projActor;

	return itemActor;
}

void Game::HandleConnection( ClientData* cd )
{
	// Create Player
	Player* player = new Player(cd);
	zPlayers[cd] = player;

	//Lets gamemode observe players.
	player->AddObserver(this->zGameMode);

	//Tells the client it has been connected.
	NetworkMessageConverter NMC;
	std::string message;

	message = NMC.Convert(MESSAGE_TYPE_CONNECTED);
	cd->Send(message);

	// Sends the world name
	message = NMC.Convert(MESSAGE_TYPE_LOAD_MAP, zWorld->GetFileName());
	cd->Send(message);

	//Send event to game so it knows what players there are.
	PlayerAddEvent PAE;
	PAE.player = player;
	NotifyObservers(&PAE);
}

void Game::HandleDisconnect( ClientData* cd )
{
		// Delete Player Behavior
		auto playerIterator = zPlayers.find(cd);
		auto playerBehavior = playerIterator->second->GetBehavior();
		
		// Create AI Behavior For Players That Disconnected
		if ( PlayerDeerBehavior* playerDeer = dynamic_cast<PlayerDeerBehavior*>(playerBehavior) )
		{
			AIDeerBehavior* aiDeer = new AIDeerBehavior(playerDeer->GetActor(), zWorld);
			zBehaviors.insert(aiDeer);
		}
		else if ( PlayerBearBehavior* playerBear = dynamic_cast<PlayerBearBehavior*>(playerBehavior) )
		{
			AIBearBehavior* aiDeer = new AIBearBehavior(playerBear->GetActor(), zWorld);
			zBehaviors.insert(aiDeer);
		}
		//Kills actor if human
		else if ( PlayerHumanBehavior* pHuman = dynamic_cast<PlayerHumanBehavior*>(playerBehavior))
		{
			Actor* pActor = pHuman->GetActor();
			dynamic_cast<BioActor*>(pActor)->Kill();
		}
		
		this->SetPlayerBehavior(playerIterator->second, NULL);

		PlayerRemoveEvent PRE;
		PRE.player = playerIterator->second;
		NotifyObservers(&PRE);

		Player* temp = playerIterator->second;
		delete temp;
		temp = NULL;
		zPlayers.erase(playerIterator);
}

void Game::HandleLootObject( ClientData* cd, std::vector<unsigned int>& actorID )
{
	std::set<Actor*> actors = this->zActorManager->GetActors();
	std::vector<Item*> lootedItems;

	auto playerIterator = this->zPlayers.find(cd);
	auto playerBehavior = playerIterator->second->GetBehavior();
	Actor* actor = playerBehavior->GetActor();
	NetworkMessageConverter NMC;
	std::string msg;
	unsigned int ID = 0;
	bool bLooted = false;
	//Loop through all actors.
	for (auto it_actor = actors.begin(); it_actor != actors.end() && !bLooted; it_actor++)
	{
		//Loop through all ID's of all actors the client tried to loot.
		for (auto it_ID = actorID.begin(); it_ID != actorID.end() && !bLooted; it_ID++)
		{
			//Check if the ID is the same.
			if ((*it_ID) == (*it_actor)->GetID())
			{
				//Check if the distance between the actors are to far to be able to loot.
				if ((actor->GetPosition() - (*it_actor)->GetPosition()).GetLength() > 7.0f)
					continue;

				//Check if the Actor is an ItemActor
				if (ItemActor* iActor = dynamic_cast<ItemActor*>(*it_actor))
				{
					msg = NMC.Convert(MESSAGE_TYPE_LOOT_OBJECT_RESPONSE, (float)iActor->GetID());
					msg += iActor->GetItem()->ToMessageString(&NMC);
					msg += NMC.Convert(MESSAGE_TYPE_ITEM_FINISHED);
					bLooted = true;
				}
				//Check if the Actor is a PlayerActor
				else if(PlayerActor* pActor = dynamic_cast<PlayerActor*>(*it_actor))
				{
					//Check if the PlayerActor is Dead.
					if (!pActor->IsAlive())
					{
						Inventory* inv = pActor->GetInventory();
						ID = pActor->GetID();

						std::vector<Item*> items = inv->GetItems();
						if (items.size() > 0)
						{
							msg = NMC.Convert(MESSAGE_TYPE_LOOT_OBJECT_RESPONSE, (float)pActor->GetID());
							for (auto it_Item = items.begin(); it_Item != items.end(); it_Item++)
							{
								msg += (*it_Item)->ToMessageString(&NMC);
								msg += NMC.Convert(MESSAGE_TYPE_ITEM_FINISHED);
							}
							bLooted = true;
						}
						
					}
				}
				//Check if the Actor is an AnimalActor.
				else if(AnimalActor* aActor = dynamic_cast<AnimalActor*>(*it_actor))
				{
					//Check if the AnimalActor is alive.
					if (!aActor->IsAlive())
					{
						//Verify that a PlayerActor was trying to loot.
						if (PlayerActor* player = dynamic_cast<PlayerActor*>(actor))
						{
							//Get the PlayerActors inventory.
							if (Inventory* playerInventory = player->GetInventory())
							{
								//Check if the PlayerActor has a Pocket Knife to be able to loot Animals.
								Item* item = playerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_MELEE, ITEM_SUB_TYPE_POCKET_KNIFE);

								if (item)
								{
									Inventory* inv = aActor->GetInventory();
									ID = aActor->GetID();

									std::vector<Item*> items = inv->GetItems();
									if (items.size() > 0)
									{
										msg = NMC.Convert(MESSAGE_TYPE_LOOT_OBJECT_RESPONSE, (float)aActor->GetID());
										for (auto it_Item = items.begin(); it_Item != items.end(); it_Item++)
										{
											msg += (*it_Item)->ToMessageString(&NMC);
											msg += NMC.Convert(MESSAGE_TYPE_ITEM_FINISHED, (float)ID);
										}
										bLooted = true;
									}
									
								}
							}
						}
					}
				}
			}
		}
	}
	if (bLooted)
		cd->Send(msg);
	else
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "No Lootable Objects Found."));
}

void Game::HandleLootItem( ClientData* cd, unsigned int itemID, unsigned int itemType, unsigned int objID, unsigned int subType )
{
	Actor* actor = this->zActorManager->GetActor(objID);
	NetworkMessageConverter NMC;
	Item* item = NULL;
	bool stacked = false;
	bool itemScattered = false;
	auto playerActor = this->zPlayers.find(cd);
	auto* pBehaviour = playerActor->second->GetBehavior();

	PlayerActor* pActor = dynamic_cast<PlayerActor*>(pBehaviour->GetActor());
	
	if(!pActor)
	{
		cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "You Are Dead Loot Failed"));
		return;
	}

	//Check if the Actor being looted is an ItemActor.
	if (ItemActor* iActor = dynamic_cast<ItemActor*>(actor))
	{
		item = iActor->GetItem();

		if (!item)
			return;

		std::string msg = NMC.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM);

		if (item->GetID() == itemID && item->GetItemType() == itemType)// && item->GetItemSubType() == subType)
		{
			if( item->GetStacking() && !pActor->GetInventory()->IsStacking(item) )
			{
				int slots = pActor->GetInventory()->CalcMaxAvailableSlots(item);

				if(item->GetStackSize() > (unsigned int)slots)
				{
					Item* new_item = NULL;

					if( Projectile* projItem  = dynamic_cast<Projectile*>(item) )
						new_item = new Projectile((*projItem));
					else if( Material* matItem = dynamic_cast<Material*>(item) )
						new_item = new Material((*matItem));
					else if( Food* foodItem = dynamic_cast<Food*>(item) )
						new_item = new Food((*foodItem));
					else if( Bandage* bandageItem = dynamic_cast<Bandage*>(item) )
						new_item = new Bandage((*bandageItem));
					else if( Misc* miscItem = dynamic_cast<Misc*>(item) )
						new_item = new Misc((*miscItem));

					if(new_item)
					{
					 	//To generate an ID. For now.
					 	Projectile projectile;
					 	new_item->SetItemID( projectile.GetID() );
					 	new_item->SetStackSize(slots);
					 	item->DecreaseStackSize(slots);

					 	item = new_item;
						itemScattered = true;
					}
					else
					{
						return;
					}
				}
			}

			msg += item->ToMessageString(&NMC);

			//add item
			if(pActor->GetInventory()->AddItem(item, stacked))
			{
				if( stacked && item->GetStackSize() == 0 )
				{
					iActor->RemoveItem();
					this->zActorManager->RemoveActor(iActor);

					SAFE_DELETE(item);
				}
				else if ( !stacked && !itemScattered )
				{
					iActor->RemoveItem();
					this->zActorManager->RemoveActor(iActor);
				}

			}
			else
			{
				cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Inventory Is Full"));
				return;
			}

			cd->Send(msg);
		}
		
	}

	//Check if the Actor being looted is a BioActor.
	else if (BioActor* bActor = dynamic_cast<BioActor*>(actor))
	{
		Inventory* inv = bActor->GetInventory();
		if (inv)
		{
			item = inv->SearchAndGetItem(itemID);

			if( !item )
				return;
		
			std::string msg = NMC.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM);
			if (item->GetItemType() == itemType)// && item->GetItemSubType() == subType)
			{
				msg += item->ToMessageString(&NMC);
				//Add item
				if(pActor->GetInventory()->AddItem(item, stacked))
				{
					bActor->GetInventory()->RemoveItem(item);

					if( stacked && item->GetStackSize() == 0 )
					{
						SAFE_DELETE(item);
					}

					if (bActor->GetInventory()->GetItems().size() <= 0)
						this->zActorManager->RemoveActor(bActor);
				}
				else
				{
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Inventory is Full"));
					return;
				}

				cd->Send(msg);
			}
			
		}
	}
}

void Game::HandleDropItem( ClientData* cd, unsigned int objectID )
{
	Actor* actor  = NULL;
	PlayerActor* pActor = NULL;
	Item* item = NULL;

	auto i = this->zPlayers.find(cd);

	actor = i->second->GetBehavior()->GetActor();
	pActor = dynamic_cast<PlayerActor*>(actor);

	if(!pActor)
		return;

	item = pActor->DropItem(objectID);

	if(!item)
		return;

	actor = NULL;
	actor = new ItemActor(item);
	this->zActorManager->AddActor(actor);
	actor->SetPosition(pActor->GetPosition());
	NetworkMessageConverter NMC;
	cd->Send(NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)item->GetID()));
}

void Game::HandleUseItem( ClientData* cd, unsigned int itemID )
{
	auto playerIterator = this->zPlayers.find(cd);
	auto playerBehavior = playerIterator->second->GetBehavior();

	Actor* actor = playerBehavior->GetActor();

	//Check if the actor trying to use an item is a PlayerActor
	if(PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor))
	{
		//Check if he has an inventory
		if (Inventory* inv = pActor->GetInventory())
		{
			NetworkMessageConverter NMC;
			Item* item = inv->SearchAndGetItem(itemID);
			std::string msg;
			if (item)
			{
				unsigned int ID = 0;
				if (Food* food = dynamic_cast<Food*>(item))
				{
					ID = food->GetID();
					if (food->Use())
					{
						//To do fix Values and stuff.
						float value = food->GetHunger();

						float fullness = pActor->GetFullness();

						pActor->SetFullness(fullness + value);
						pActor->HungerHasChanged();

						//Sending Message to client And removing stack from inventory.
						inv->RemoveItemStack(ID, 1);
						msg = NMC.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);

						cd->Send(msg);
					}
					else
					{
						msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Food_Stack_is_Empty");
						cd->Send(msg);
					}
					if (food->GetStackSize() <= 0)
					{
						item = inv->RemoveItem(food);

						if(item)
						{
							msg = NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
							cd->Send(msg);
							
							delete item, item = NULL;
						}
					}
				}
				else if (Container* container = dynamic_cast<Container*>(item))
				{
					if (container->Use())
					{
						//To do fix values and stuff
						float hydration = 2.0f + pActor->GetHydration();

						pActor->SetHydration(hydration);
						ID = container->GetID();
						msg = NMC.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
						cd->Send(msg);
					}
					else
					{
						msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Container_Stack_is_Empty");
						cd->Send(msg);
					}
				}
				else if(Bandage* bandage = dynamic_cast<Bandage*>(item))
				{
					ID = bandage->GetID();
					
					if (bandage->Use())
					{				
						float bleedingLevel = pActor->GetBleeding();
						if(bandage->GetItemSubType() == 0) //Used poor bandage
						{
							pActor->SetBleeding(bleedingLevel - 1);
						}
						else if(bandage->GetItemSubType() == 1) //Used great bandage
						{
							pActor->SetBleeding(bleedingLevel - 3);
						}
						//pActor->SetBleeding(false);
						
						//Sending Message to client And removing stack from inventory.
						inv->RemoveItemStack(ID, 1);
						msg = NMC.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);

						cd->Send(msg);
					}
					else
					{
						msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Bandage_Stack_is_Empty");
						cd->Send(msg);
					}
					if (bandage->GetStackSize() <= 0)
					{
						item = inv->RemoveItem(bandage);

						if(item)
						{
							msg = NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
							cd->Send(msg);
							delete item, item = NULL;
						}
					}
				}
			}
		}
	}
}

void Game::HandleUseWeapon( ClientData* cd, unsigned int itemID )
{
	Actor* actor = NULL;

	auto playerIterator = zPlayers.find(cd);
	actor = playerIterator->second->GetBehavior()->GetActor();

	PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor);
	if ( !(pActor) )
	{
		MaloW::Debug("Actor cannot be found in Game.cpp, onEvent, PlayerUseEquippedWeaponEvent.");
		return;
	}

	Inventory* inventory = pActor->GetInventory();
	if( !(inventory) )
	{
		MaloW::Debug("Inventory is null in Game.cpp, onEvent, PlayerUseEquippedWeaponEvent.");
		return;
	}

	Item* item = inventory->GetPrimaryEquip();
	if ( !(item ) )
	{
		MaloW::Debug("Item is null in Game.cpp, onEvent, PlayerUseEquippedWeaponEvent.");
		return;
	}
	if( item->GetID() != itemID )
	{
		MaloW::Debug("Item ID do not match in Game.cpp, onEvent, PlayerUseEquippedWeaponEvent.");
		return;
	}
	NetworkMessageConverter NMC;

	if(RangedWeapon* ranged = dynamic_cast<RangedWeapon*>(item))
	{
		if (ranged->GetItemSubType() == ITEM_SUB_TYPE_BOW)
		{
			//Check if arrows are equipped
			Projectile* arrow = inventory->GetProjectile();
			if(arrow && arrow->GetItemSubType() == ITEM_SUB_TYPE_ARROW)
			{
				//create projectileActor
				PhysicsObject* pObj = this->zPhysicsEngine->CreatePhysicsObject(arrow->GetModel());
				ProjectileActor* projActor = new ProjectileActor(pActor, pObj);
		
				ProjectileArrowBehavior* projBehavior = NULL;
				Damage damage;

				//Sets damage
				damage.piercing = ranged->GetDamage() + arrow->GetDamage();
				projActor->SetDamage(damage);
				//Set other values
				projActor->SetScale(projActor->GetScale());
				projActor->SetPosition( pActor->GetPosition() + pActor->GetCameraOffset());
				projActor->SetDir(pActor->GetDir());

				//Create behavior
				projBehavior = new ProjectileArrowBehavior(projActor, this->zWorld);
		
				//Set Nearby actors
				std::set<Actor*> actors;
				this->zActorManager->GetCollideableActorsInCircle(actor->GetPosition().GetXZ(), projBehavior->GetCollisionRadius(), actors);
				projBehavior->SetNearActors(actors);

				//Adds the actor and Behavior
				this->zActorManager->AddActor(projActor);
				this->zBehaviors.insert(projBehavior);
				//Decrease stack
				arrow->Use();
				inventory->RemoveItemStack(arrow->GetID(), 1);


				//if arrow stack is empty
				if (arrow->GetStackSize() <= 0)
				{
					std::string msg = NMC.Convert(MESSAGE_TYPE_REMOVE_EQUIPMENT, (float)arrow->GetID());
					msg += NMC.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)EQUIPMENT_SLOT_PROJECTILE);
					cd->Send(msg);
					item = inventory->RemoveItem(arrow);
					SAFE_DELETE(item);
					inventory->UnEquipProjectile();
				}
				//Send feedback message
				cd->Send(NMC.Convert(MESSAGE_TYPE_WEAPON_USE, (float)ranged->GetID()));

				std::string msg = NMC.Convert(MESSAGE_TYPE_PLAY_SOUND, "Media/Sound/BowShot.mp3");
				msg += NMC.Convert(MESSAGE_TYPE_POSITION, pActor->GetPosition());
				this->SendToAll(msg);

				//Send Message to client to Play Shot Bow Animation
				msg = NMC.Convert(MESSAGE_TYPE_PLAY_ANIMATION, IDLE_O1);
				msg += NMC.Convert(MESSAGE_TYPE_OBJECT_ID, (float)pActor->GetID()); 
				this->SendToAll(msg);
			}
			else
				cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "No_Arrows_Equipped"));
		}
	}
	else if(Projectile* proj = dynamic_cast<Projectile*>(item))
	{
		//TODO: Implement rocks
	}
	else if(MeleeWeapon* meele = dynamic_cast<MeleeWeapon*>(item))
	{
		float range = 0.0f; 
		BioActor* victim = NULL;

		//Check Collisions
		range = meele->GetRange();
		victim = dynamic_cast<BioActor* >(this->zActorManager->CheckCollisions(pActor, range));

		if(victim)
		{
			Damage dmg;

			if(meele->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
				dmg.slashing = meele->GetDamage();

			victim->TakeDamage(dmg, pActor);

			//Send Message to client to Play Cut Animation
			std::string msg = NMC.Convert(MESSAGE_TYPE_PLAY_ANIMATION, "idle_01");
			msg += NMC.Convert(MESSAGE_TYPE_OBJECT_ID, (float)pActor->GetID()); 
			this->SendToAll(msg);
		}
	}
}

void Game::HandleCraftItem( ClientData* cd, unsigned int itemID )
{
	auto playerIterator = this->zPlayers.find(cd);
	auto playerBehavior = playerIterator->second->GetBehavior();

	Actor* actor = playerBehavior->GetActor();
	unsigned int craftType = ITEM_TYPE_PROJECTILE;
	unsigned int craftSubType = ITEM_SUB_TYPE_ARROW;
	if(PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor))
	{
		if (Inventory* inv = pActor->GetInventory())
		{
			NetworkMessageConverter NMC;
			Item* item = inv->SearchAndGetItem(itemID);
			std::string msg;
			int stackRemoved = 0;
			if (item)
			{
				CraftedTypes craftedType = CraftedTypes(craftType, craftSubType);
				//Items used for crafting and the required stacks.
				std::map<Item*, unsigned int> item_stack_out;

				//Check if there are enough materials to Craft.
				if(this->zCraftingManager->Craft(inv, &craftedType, item_stack_out))
				{
					Item* craftedItem = NULL;
					if (craftedType.type == ITEM_TYPE_PROJECTILE && craftedType.subType == ITEM_SUB_TYPE_BOW)
					{
						const Projectile* temp_Item = GetItemLookup()->GetProjectile(craftSubType);
						craftedItem = new Projectile((*temp_Item));
					}
					else if (craftedType.type == ITEM_TYPE_WEAPON_RANGED && craftedType.subType == ITEM_SUB_TYPE_BOW)
					{
						const RangedWeapon* temp_Item = GetItemLookup()->GetRangedWeapon(craftSubType);
						craftedItem = new RangedWeapon((*temp_Item));
					}
					else if (craftedType.type == ITEM_TYPE_MISC && craftedType.subType == ITEM_SUB_TYPE_REGULAR_TRAP)
					{
						const Misc* temp_Item = GetItemLookup()->GetTrap(craftSubType);
						craftedItem = new Misc((*temp_Item));
					}
					else if (craftedType.type == ITEM_TYPE_MISC && craftedType.subType == ITEM_SUB_TYPE_CAMPFIRE)
					{
						const Misc* temp_Item = GetItemLookup()->GetTrap(craftSubType);
						craftedItem = new Misc((*temp_Item));
					}
					if (craftedItem)
					{
						int newWeightChange = craftedItem->GetStackSize() * craftedItem->GetWeight();
						for (auto it = item_stack_out.begin(); it != item_stack_out.end(); it++)
						{
							newWeightChange -= (it->first->GetWeight() * it->second);
						}

						//Check if the new Weight is less or equal to the max Weight.
						if(inv->GetTotalWeight() + newWeightChange <= inv->GetInventoryCapacity())
						{
							for (auto it = item_stack_out.begin(); it != item_stack_out.end(); it++)
							{
								//Decrease stacks for the Material.
								it->first->DecreaseStackSize(it->second);
								//Check if material should be removed or just remove stack.
								if (it->first->GetStackSize() > 0)
								{
									inv->RemoveItemStack(it->first->GetID(), it->second);
								}
								else
								{
									inv->RemoveItem(it->first);
									cd->Send(NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, it->first->GetID()));
								}
								
							}
							//Try to add the crafted item to the inventory.
							bool stacked = false;
							if (inv->AddItem(craftedItem, stacked))
							{
								//Send Add Inventory Msg to the Player.
								std::string msg = NMC.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM);
								msg += craftedItem->ToMessageString(&NMC);
								cd->Send(msg);

								if (stacked)
								{
									if (craftedItem->GetStackSize() <= 0)
										SAFE_DELETE(craftedItem);
								}

								//Loop through items again and send Craft msg
								for (auto it = item_stack_out.begin(); it != item_stack_out.end(); it++)
								{
									msg = NMC.Convert(MESSAGE_TYPE_ITEM_CRAFT, (float)it->first->GetID());
									msg += NMC.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)it->second);
									cd->Send(msg);
									if (it->first->GetStackSize() <= 0)
									{
										Item* temp = it->first;
										SAFE_DELETE(temp);
									}
								}
							}
							else
							{
								for (auto it = item_stack_out.begin(); it != item_stack_out.end(); it++)
								{
									inv->RemoveItem(it->first);
									
									it->first->IncreaseStackSize(it->second);
									if(inv->AddItem(it->first, stacked))
									{
										if (stacked)
										{
											MaloW::Debug("Weird Error When Crafting, item stacked but shouldn't have");
											Item* temp = it->first;
											SAFE_DELETE(temp);
										}
									}
									else
										MaloW::Debug("Weird Error When Crafting, item Can't be re added to inventory");
										
								}
								cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not enough space in inventory"));
							}
						}
					}
				}
				else
					cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not enough materials to craft"));
				
				/*unsigned int ID = 0;
				if(Material* material = dynamic_cast<Material*>(item))
				{
					if (material->GetItemSubType() == ITEM_SUB_TYPE_SMALL_STICK)
					{
						if (material->IsUsable())
						{
							//Use the Materials.
							material->Use();
							//Send message to client to use the Material.
							ID = material->GetID();
							msg = NMC.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
							cd->Send(msg);

							//Get the number of Materials Being Removed.
							stackRemoved = material->GetRequiredStacksToCraft();
							//Remove stacks from inventory.
							inv->RemoveItemStack(ID, stackRemoved);

							//Check if there are no stacks left
							if (material->GetStackSize() <= 0)
							{
								//Remove the Item from the Inventory
								if (inv->RemoveItem(material))
								{
									//Send Message to Client
									ID = material->GetID();
									msg = NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
									cd->Send(msg);

									SAFE_DELETE(material);
								}
							}

							//Create a bow With default Values
							const Projectile* temp_Arrow = GetItemLookup()->GetProjectile(ITEM_SUB_TYPE_ARROW);

							if (temp_Arrow)
							{
								Projectile* new_Arrow = new Projectile((*temp_Arrow));

								bool stacked;
								if (inv->AddItem(new_Arrow, stacked))
								{
									msg = NMC.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM);
									msg += new_Arrow->ToMessageString(&NMC);
									cd->Send(msg);

									if( stacked && new_Arrow->GetStackSize() == 0 )
										SAFE_DELETE(new_Arrow);
								}
							}
						}
						else
						{
							msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not_Enough_Materials_To_Craft");
							cd->Send(msg);
						}	
					}
					else if (material->GetItemSubType() == ITEM_SUB_TYPE_MEDIUM_STICK || material->GetItemSubType() == ITEM_SUB_TYPE_THREAD)
					{
						Item* tempItem = NULL;
						
						//Get the Correct Secondary Material
						if (material->GetItemSubType() == ITEM_SUB_TYPE_MEDIUM_STICK)
						{
							tempItem = inv->SearchAndGetItemFromType(ITEM_TYPE_MATERIAL, ITEM_SUB_TYPE_THREAD);
						}
						else if (material->GetItemSubType() == ITEM_SUB_TYPE_THREAD)
						{
							tempItem = inv->SearchAndGetItemFromType(ITEM_TYPE_MATERIAL, ITEM_SUB_TYPE_MEDIUM_STICK);
						}

						if (Material* material_Secondary = dynamic_cast<Material*>(tempItem))
						{
							if (material->IsUsable() && material_Secondary->IsUsable())
							{
								//Use Primary material

								//Use the Materials.
								material->Use();
								//Send message to client to use the Material.
								ID = material->GetID();
								msg = NMC.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
								cd->Send(msg);
								//Get the number of Materials Being Removed.
								stackRemoved = material->GetRequiredStacksToCraft();
								//Remove stacks from inventory.
								inv->RemoveItemStack(ID, stackRemoved);

								//Use Secondary material

								//Use the Materials.
								material_Secondary->Use();
								//Send message to client to use the Material.
								ID = material_Secondary->GetID();
								msg = NMC.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
								cd->Send(msg);

								//Get the number of Materials Being Removed.
								stackRemoved = material_Secondary->GetRequiredStacksToCraft();
								//Remove stacks from inventory.
								inv->RemoveItemStack(ID, stackRemoved);

								//Check if there are no stacks left
								if (material->GetStackSize() <= 0)
								{
									//Remove the Item from the Inventory
									if (inv->RemoveItem(material))
									{
										//Send Message to Client
										ID = material->GetID();
										msg = NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
										cd->Send(msg);

										SAFE_DELETE(material);
									}
								}
								//Check if there are no stacks left
								if (material_Secondary->GetStackSize() <= 0)
								{
									//Remove the Item from the Inventory
									if (inv->RemoveItem(material_Secondary))
									{
										//Send Message to Client
										ID = material_Secondary->GetID();
										msg = NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)ID);
										cd->Send(msg);

										SAFE_DELETE(material_Secondary);
									}
								}
								//Craft a bow With default Values
								const RangedWeapon* temp_bow = GetItemLookup()->GetRangedWeapon(ITEM_SUB_TYPE_BOW);

								if (temp_bow)
								{
									RangedWeapon* new_Bow = new RangedWeapon((*temp_bow));
									bool stacked = false;
									if (inv->AddItem(new_Bow, stacked))
									{
										msg = NMC.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM);
										msg += new_Bow->ToMessageString(&NMC);
										cd->Send(msg);

										if( stacked && new_Bow->GetStackSize() == 0 )
											SAFE_DELETE(new_Bow);
									}
								}
							}
							else
							{
								msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Not_Enough_Materials_To_Craft");
								cd->Send(msg);
							}
						}
						else
						{
							msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Missing_materials_in_order_to_craft_Bow");
							cd->Send(msg);
						}
					}
				}*/
			}
		}
	}
}

void Game::HandleEquipItem( ClientData* cd, unsigned int itemID )
{
	std::string msg;
	int slot = -1;

	NetworkMessageConverter NMC;
	Actor* actor = this->zPlayers[cd]->GetBehavior()->GetActor();
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor);
	if (!pActor)
	{
		msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Equipping items is something only humans can do");
		cd->Send(msg);
		return;
	}
	Inventory* inventory = pActor->GetInventory();
	Item* item = inventory->SearchAndGetItem(itemID);
	Item* ret = NULL;
	bool success = false;

	if(Projectile* proj = dynamic_cast<Projectile*>(item))
	{
		int weigth = inventory->GetTotalWeight();

		ret = inventory->EquipProjectile(proj);
		
		if(weigth > inventory->GetTotalWeight())
		{
			Item* temp = inventory->RemoveItem(proj);
			SAFE_DELETE(temp);
		}

		slot = EQUIPMENT_SLOT_PROJECTILE;
	}
	else if(MeleeWeapon* meele = dynamic_cast<MeleeWeapon*>(item))
	{
		ret = inventory->EquipMeleeWeapon(meele, success);
		slot = EQUIPMENT_SLOT_MELEE_WEAPON;
	}
	else if(RangedWeapon* ranged = dynamic_cast<RangedWeapon*>(item))
	{
		ret = inventory->EquipRangedWeapon(ranged, success);
		slot = EQUIPMENT_SLOT_RANGED_WEAPON;
	}

	if(!success && slot != EQUIPMENT_SLOT_PROJECTILE)
	{
		msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Cannot Equip Item");
		cd->Send(msg);
		return;
	}
	//Check if the Equipped Item is the Primary one Then Add it to the Mesh
	Item* primaryWpn = inventory->GetPrimaryEquip();
	if (primaryWpn == item)
		this->HandleBindings(item, pActor->GetID());
	
	msg = NMC.Convert(MESSAGE_TYPE_EQUIP_ITEM, (float)item->GetID());
	msg += NMC.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)slot);
	cd->Send(msg);
}

void Game::HandleUnEquipItem( ClientData* cd, unsigned int itemID, int eq_slot )
{
	std::string msg;
	NetworkMessageConverter NMC;
	Actor* actor = this->zPlayers[cd]->GetBehavior()->GetActor();
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor);
	if (!pActor)
		return;

	Inventory* inventory = pActor->GetInventory();
	Item* item = inventory->SearchAndGetItem(itemID);

	bool wasPrimary = false;

	if (item == inventory->GetPrimaryEquip())
		wasPrimary = true;
	
	if(!item)
	{
		msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "Item Was Not Found");
		cd->Send(msg);
		return;
	}

	if(eq_slot == EQUIPMENT_SLOT_PROJECTILE)
	{
		inventory->UnEquipProjectile();
	}
	else if(eq_slot == EQUIPMENT_SLOT_MELEE_WEAPON)
	{
		inventory->UnEquipMeleeWeapon();
	}
	else if(eq_slot == EQUIPMENT_SLOT_RANGED_WEAPON)
	{
		inventory->UnEquipRangedWeapon();
	}
	else
	{
		msg = NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, "No Such slot");
		cd->Send(msg);
		return;
	}

	if (wasPrimary)
	{
		msg = NMC.Convert(MESSAGE_TYPE_MESH_UNBIND, (float)pActor->GetID());
		msg += NMC.Convert(MESSAGE_TYPE_MESH_MODEL, item->GetModel());
		cd->Send(msg);

		Item* newPrimary = inventory->GetPrimaryEquip();

		if (newPrimary)
			this->HandleBindings(newPrimary, pActor->GetID());
	}
	
	msg = NMC.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)itemID);
	msg += NMC.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)eq_slot);
	cd->Send(msg);
}

void Game::HandleBindings(Item* item, const unsigned int ID)
{
	std::string msg;
	NetworkMessageConverter NMC;

	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
	{
		if (item->GetItemSubType() == ITEM_SUB_TYPE_BOW)
		{
			msg = NMC.Convert(MESSAGE_TYPE_MESH_BINDING, BONE_L_WEAPON);
			msg += NMC.Convert(MESSAGE_TYPE_MESH_MODEL, item->GetModel());
			msg += NMC.Convert(MESSAGE_TYPE_OBJECT_ID, (float)ID);
			this->SendToAll(msg);
		}

	}
	else if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
	{
		if (item->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
		{
			msg = NMC.Convert(MESSAGE_TYPE_MESH_BINDING, BONE_L_WEAPON);
			msg += NMC.Convert(MESSAGE_TYPE_MESH_MODEL, item->GetModel());
			msg += NMC.Convert(MESSAGE_TYPE_OBJECT_ID, (float)ID);
			this->SendToAll(msg);
		}
		else if (item->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
		{
			msg = NMC.Convert(MESSAGE_TYPE_MESH_BINDING, BONE_L_WEAPON);
			msg += NMC.Convert(MESSAGE_TYPE_MESH_MODEL, item->GetModel());
			msg += NMC.Convert(MESSAGE_TYPE_OBJECT_ID, (float)ID);
			this->SendToAll(msg);
		}
	}
	else if (item->GetItemType() == ITEM_TYPE_PROJECTILE && item->GetItemSubType() == ITEM_SUB_TYPE_ROCK)
	{
		msg = NMC.Convert(MESSAGE_TYPE_MESH_BINDING, BONE_L_WEAPON);
		msg += NMC.Convert(MESSAGE_TYPE_MESH_MODEL, item->GetModel());
		msg += NMC.Convert(MESSAGE_TYPE_OBJECT_ID, (float)ID);
		this->SendToAll(msg);
	}
}

void Game::SendToAll( std::string msg)
{
	for(auto it = this->zPlayers.begin(); it != this->zPlayers.end(); it++)
	{
		it->first->Send(msg);
	}
}

void Game::ModifyLivingPlayers( const int value )
{
	this->zPlayersAlive += value;

	this->zCurrentFogEnclosement = ( this->zInitalFogEnclosement + (this->zIncrementFogEnclosement * this->zPlayersAlive) ) * this->zFogTotalDecreaseCoeff;
	
	NetworkMessageConverter NMC;
	std::string message = NMC.Convert(MESSAGE_TYPE_FOG_ENCLOSEMENT, this->zCurrentFogEnclosement);
	this->SendToAll(message);
}

void Game::RestartGame()
{
	NetworkMessageConverter NMC;
	std::string msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, "ServerRestarting");
	SendToAll(msg);
	msg = NMC.Convert(MESSAGE_TYPE_SERVER_RESTART);
	SendToAll(msg);

	//Delete All Actors
	this->zActorManager->ClearAll();
	//Remove old messages
	this->zSyncher->ClearAll();

	//Recreate Actors
	std::string message = "";
	int increment = 1;
	for (auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		/*Delete old Behavior*/
		SetPlayerBehavior( (*it).second, 0 );
		(*it).second->GetKeys().ClearStates();

		PhysicsObject* physObj = zPhysicsEngine->CreatePhysicsObject("Media/Models/temp_guy.obj");
		physObj->SetModel( (*it).second->GetModelPath() );

		PlayerActor* pActor = new PlayerActor((*it).second, physObj);
		PlayerHumanBehavior* pBehavior = new PlayerHumanBehavior(pActor, zWorld, (*it).second);

		pActor->SetPosition(CalcPlayerSpawnPoint(32));
		pActor->SetScale(pActor->GetScale());
		pActor->AddObserver(this->zGameMode);
		SetPlayerBehavior((*it).second, pBehavior);
		this->zActorManager->AddActor(pActor);

		//Should be changed Later
		auto offsets = this->zCameraOffset.find((*it).second->GetModelPath());

		if(offsets != this->zCameraOffset.end())
			dynamic_cast<PlayerActor*>(pActor)->SetCameraOffset(offsets->second);

		message = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)pActor->GetID());
		message += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, (float)1);
		(*it).first->Send(message);
	}
	//Debug
	//SpawnAnimalsDebug();

	//Set everyone to false
	for (auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		(*it).second->SetReady(false);
	}

	SpawnItemsDebug();
	//SpawnAnimalsDebug();
	SpawnHumanDebug();

}