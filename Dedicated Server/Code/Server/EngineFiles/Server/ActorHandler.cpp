#include "ActorHandler.h"

ActorHandler::ActorHandler()
{
	this->zObjManager = new ObjectManager();
	this->zObjManager->ReadObjects();
	this->zPhysicsEngine = new PhysicsEngine();
}

ActorHandler::~ActorHandler()
{
	for(auto x = this->zPlayers.begin(); x < this->zPlayers.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zAnimals.begin(); x < this->zAnimals.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zFoods.begin(); x < this->zFoods.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zWeapons.begin(); x < this->zWeapons.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zStaticProjectiles.begin(); x < this->zStaticProjectiles.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto x = this->zDynamicProjectiles.begin(); x < this->zDynamicProjectiles.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	SAFE_DELETE(this->zObjManager);
	SAFE_DELETE(this->zPhysicsEngine);
}

void ActorHandler::UpdateObjects( float deltaTime )
{
	//Update Players
	for (auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
	//Update Animals
	for (auto it = this->zAnimals.begin(); it < this->zAnimals.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
	//Update DynamicObjects
	for (auto it = this->zDynamicProjectiles.begin(); it < this->zDynamicProjectiles.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
}

bool ActorHandler::AddNewPlayer(PlayerActor* new_player)
{
	if(!new_player)
		return false;

	PhysicsObject* pObj = this->zPhysicsEngine->CreatePhysicsObject(new_player->GetActorModel(), new_player->GetPosition());
	new_player->SetPhysicObj(pObj);
	new_player->SetObjManager(this->zObjManager);
	this->zPlayers.push_back(new_player);

	if(!pObj)
		MaloW::Debug("Error in function AddNewPlayer in ActorHandler: PhysicObj is null.");

	return true;
}

bool ActorHandler::AddNewStaticFoodActor( FoodObject* new_Food )
{
	if(!new_Food)
		return false;

	this->zFoods.push_back(new_Food);

	return true;
}

bool ActorHandler::AddNewStaticWeaponActor( WeaponObject* new_Weapon )
{
	if(!new_Weapon)
		return false;

	this->zWeapons.push_back(new_Weapon);

	return true;
}

bool ActorHandler::AddNewStaticContainerActor( ContainerObject* new_Container )
{
	if(!new_Container)
		return false;

	this->zContainers.push_back(new_Container);

	return true;
}

bool ActorHandler::AddNewStaticProjectileActor( StaticProjectileObject* new_Projectile )
{
	if(!new_Projectile)
		return false;

	this->zStaticProjectiles.push_back(new_Projectile);

	return true;
}

bool ActorHandler::AddNewDynamicProjectileActor( DynamicProjectileObject* new_Projectile )
{
	if(!new_Projectile)
		return false;

	PhysicsObject* pObj = this->zPhysicsEngine->CreatePhysicsObject(new_Projectile->GetActorModel(), 
																		new_Projectile->GetPosition());
	new_Projectile->SetPhysicObject(pObj);
	this->zDynamicProjectiles.push_back(new_Projectile);

	if(!pObj)
		MaloW::Debug("Error in function AddNewDynamicProjectileActor in ActorHandler: PhysicObj is null.");

	return true;
}

Actor* ActorHandler::GetActor( const int ID, const int TYPE ) const
{
	Actor* ac = NULL;
	this->SearchForActor(ID, TYPE, &ac);

	return ac;
}

bool ActorHandler::RemovePlayerActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_PLAYER);

	if(index == -1)
		return false;

	PlayerActor* temp = this->zPlayers[index];
	this->zPlayers.erase(this->zPlayers.begin() + index);
	this->zPhysicsEngine->DeletePhysicsObject(temp->GetPhysicObject());
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveAnimalActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_ANIMAL);

	if(index == -1)
		return false;
	
	AnimalActor* temp = this->zAnimals[index];
	this->zAnimals.erase(this->zAnimals.begin() + index);

	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticFoodActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_FOOD);

	if(index == -1)
		return false;

	FoodObject* temp = this->zFoods[index];
	this->zFoods.erase(this->zFoods.begin() + index);
	
	SAFE_DELETE(temp);
	
	return true;
}

bool ActorHandler::RemoveStaticWeaponActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_WEAPON);

	if(index == -1)
		return false;

	WeaponObject* temp = this->zWeapons[index];

	this->zWeapons.erase(this->zWeapons.begin() + index);
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticContainerActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_CONTAINER);

	if(index == -1)
		return false;

	ContainerObject* temp = this->zContainers[index];

	this->zContainers.erase(this->zContainers.begin() + index);
	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveStaticProjectileActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_STATIC_OBJECT_PROJECTILE);

	if(index == -1)
		return false;

	StaticProjectileObject* temp = this->zStaticProjectiles[index];

	this->zStaticProjectiles.erase(this->zStaticProjectiles.begin() + index);

	SAFE_DELETE(temp);

	return true;
}

bool ActorHandler::RemoveDynamicProjectileActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE);

	if(index == -1)
		return false;

	DynamicProjectileObject* temp = this->zDynamicProjectiles[index];

	this->zDynamicProjectiles.erase(this->zDynamicProjectiles.begin() + index);
	this->zPhysicsEngine->DeletePhysicsObject(temp->GetPhysicObject());
	SAFE_DELETE(temp);

	return true;
}

const int ActorHandler::SearchForActor( const int ID, int TYPE ) const
{
	if(TYPE == ACTOR_TYPE_PLAYER)
	{
		for (unsigned int it = 0; it < this->zPlayers.size(); it++)
		{
			if(this->zPlayers[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_ANIMAL)
	{
		for (unsigned int it = 0; it < this->zAnimals.size(); it++)
		{
			if(this->zAnimals[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_FOOD)
	{
		for (unsigned int it = 0; it < this->zFoods.size(); it++)
		{
			if(this->zFoods[it]->GetID() == ID)
				return it;
		}

	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_WEAPON)
	{
		for (unsigned int it = 0; it < this->zWeapons.size(); it++)
		{
			if(this->zWeapons[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_CONTAINER)
	{
		for (unsigned int it = 0; it < this->zContainers.size(); it++)
		{
			if(this->zContainers[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zStaticProjectiles.size(); it++)
		{
			if(this->zStaticProjectiles[it]->GetID() == ID)
				return it;
		}
	}

	else if(TYPE == ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zDynamicProjectiles.size(); it++)
		{
			if(this->zDynamicProjectiles[it]->GetID() == ID)
				return it;
		}
	}

	return -1;
}

const int ActorHandler::SearchForActor( const int ID, int TYPE, Actor** aOut ) const
{
	if(TYPE == ACTOR_TYPE_PLAYER)
	{
		for (unsigned int it = 0; it < this->zPlayers.size(); it++)
		{
			if(this->zPlayers[it]->GetID() == ID)	
			{
				*aOut = this->zPlayers[it];
				return it;
			}
		}

	}

	else if(TYPE == ACTOR_TYPE_ANIMAL)
	{
		for (unsigned int it = 0; it < this->zAnimals.size(); it++)
		{
			if(this->zAnimals[it]->GetID() == ID)
			{
				*aOut = this->zAnimals[it];
				return it;
			}
		}

	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_FOOD)
	{
		for (unsigned int it = 0; it < this->zFoods.size(); it++)
		{
			if(this->zFoods[it]->GetID() == ID)
			{
				*aOut = this->zFoods[it];
				return it;
			}
		}

	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_WEAPON)
	{
		for (unsigned int it = 0; it < this->zWeapons.size(); it++)
		{
			if(this->zWeapons[it]->GetID() == ID)
			{
				*aOut = this->zWeapons[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_CONTAINER)
	{
		for (unsigned int it = 0; it < this->zContainers.size(); it++)
		{
			if(this->zContainers[it]->GetID() == ID)
			{
				*aOut = this->zContainers[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_STATIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zStaticProjectiles.size(); it++)
		{
			if(this->zStaticProjectiles[it]->GetID() == ID)
			{
				*aOut = this->zStaticProjectiles[it];
				return it;
			}
		}
	}

	else if(TYPE == ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zDynamicProjectiles.size(); it++)
		{
			if(this->zDynamicProjectiles[it]->GetID() == ID)
			{
				*aOut = this->zDynamicProjectiles[it];
				return it;
			}
		}
	}

	aOut = NULL;
	return -1;
}

ObjectManager* ActorHandler::GetObjManager() const
{
	return this->zObjManager;
}

/*Not Complete*/
const std::vector<CollisionEvent>& ActorHandler::CheckCollisions()
{

	std::vector<CollisionEvent> cEvent;

	/*TESTS PLAYERS VS PLAYERS*/
	std::vector<PlayerActor*> pCol;

	for(auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		PlayerVsPlayers(*it, pCol);

		if(pCol.size() > 0)
			(*it)->RewindPosition();

		pCol.clear();
	}

	/*TESTS PLAYERS VS PROJECTILES*/
	std::vector<DynamicProjectileObject*> dCol; 
	std::vector<PhysicsCollisionData> pcd;

	for(auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		PlayerVsDynamics(*it, dCol, pcd);

		if(dCol.size() != 0)
		{
			float totalDamageTaken = 0.0f;

			for (auto it_s = dCol.begin(); it_s < dCol.end(); it_s++)
			{
				if((*it)->IsAlive())
				{
					totalDamageTaken += (*it_s)->GetDamage();
					if((*it)->TakeDamage(totalDamageTaken))
					{
						CollisionEvent ce;
						ce.actor_aggressor_ID	= (*it_s)->GetObjPlayerOwner();
						ce.actor_victim_ID		= (*it)->GetID();
						ce.actor_victim_type	= ACTOR_TYPE_PLAYER;
						ce.event_type			= COLLISION_EVENT_DEATH;
						cEvent.push_back(ce);
					}
				}

				(*it_s)->SetMoving(false);
			}

		}
	}

	return cEvent;
}

void ActorHandler::PlayerVsPlayers( PlayerActor* pTest, std::vector<PlayerActor*> &pCollide )
{
	PhysicsObject* pTemp = pTest->GetPhysicObject();
	PhysicsCollisionData pcd;

	for(auto it = this->zPlayers.begin(); it < this->zPlayers.end(); it++)
	{
		if(!(*it)->IsAlive())
			continue;

		if((*it) != pTest)
		{
			pcd = this->zPhysicsEngine->GetCollision(pTemp, (*it)->GetPhysicObject());
			
			if(pcd.collision)
				pCollide.push_back(*it);
		}
		
	}
}

void ActorHandler::PlayerVsDynamics( PlayerActor* pTest, std::vector<DynamicProjectileObject*> &pCollide, 
									std::vector<PhysicsCollisionData> &pcd )
{
	PhysicsObject *pTemp = pTest->GetPhysicObject();
	PhysicsCollisionData temp_pcd;

	for(auto it = this->zDynamicProjectiles.begin(); it < this->zDynamicProjectiles.end(); it++)
	{
		if(!(*it)->IsMoving())
			continue;		
		
		temp_pcd = this->zPhysicsEngine->GetCollision(pTemp, (*it)->GetPhysicObject());

		if (temp_pcd.collision)
		{
			pCollide.push_back(*it);
			pcd.push_back(temp_pcd);
		}

	}
}


