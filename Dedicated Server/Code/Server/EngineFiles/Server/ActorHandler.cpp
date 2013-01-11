#include "ActorHandler.h"

ActorHandler::ActorHandler()
{
	this->zObjManager = new ObjectManager();
	this->zObjManager->ReadObjects();
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
	if (this->zObjManager)
	{
		delete this->zObjManager;
		this->zObjManager = NULL;
	}
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
	for (auto it = this->zProjectiles.begin(); it < this->zProjectiles.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
}

bool ActorHandler::AddNewPlayer(PlayerActor* new_player)
{
	if(!new_player)
		return false;

	new_player->SetObjManager(this->zObjManager);
	this->zPlayers.push_back(new_player);

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

bool ActorHandler::AddNewDynamicProjectileActor( DynamicProjectileObject* new_Projectile )
{
	if(!new_Projectile)
		return false;

	this->zProjectiles.push_back(new_Projectile);

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

bool ActorHandler::RemoveDynamicActor( const int ID )
{
	int index = this->SearchForActor(ID, ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE);

	if(index == -1)
		return false;

	DynamicProjectileObject* temp = this->zProjectiles[index];

	this->zProjectiles.erase(this->zProjectiles.begin() + index);

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

	else if(TYPE == ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zProjectiles.size(); it++)
		{
			if(this->zProjectiles[it]->GetID() == ID)
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

	else if(TYPE == ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE)
	{
		for (unsigned int it = 0; it < this->zProjectiles.size(); it++)
		{
			if(this->zProjectiles[it]->GetID() == ID)
			{
				*aOut = this->zProjectiles[it];
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