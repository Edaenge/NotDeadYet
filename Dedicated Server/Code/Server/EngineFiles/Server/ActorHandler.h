/*
Made by Edänge Simon 2012-12-17
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "../../../../../Source/MaloWLib/Safe.h"
#include "PlayerActor.h"
#include "AnimalActor.h"
#include "ObjectManager.h"
#include "DynamicProjectileObject.h"
#include "../../../../../Source/PhysicsEngine/PhysicsEngine.h"

static const enum ACTOR_TYPE
{
	ACTOR_TYPE_PLAYER,
	ACTOR_TYPE_ANIMAL,
	ACTOR_TYPE_STATIC_OBJECT_FOOD,
	ACTOR_TYPE_STATIC_OBJECT_WEAPON,
	ACTOR_TYPE_STATIC_OBJECT_CONTAINER,
	ACTOR_TYPE_STATIC_OBJECT_PROJECTILE,
	ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE
};

struct CollisionEvent
{
	int actor_aggressor_ID;
	int actor_victim_ID;
	unsigned int actor_aggressor_type;
	unsigned int actor_victim_type;
	unsigned int event_type;
};

static const float MAX_COLLISION_DISTANCE_PLAYER = 1.0f;

class ActorHandler
{

public:
	ActorHandler();
	virtual ~ActorHandler();

	void UpdateObjects(float deltaTime);
	/*! Checks collisions against objects.*/
	void CheckCollisions();
	/*! pCollide lists all collisions.
		Rewinds target BioActor in actors if there was a collision.
		pTest will not be Rewinded.
	*/
	void BioActorVSBioActor(BioActor* pTest, std::vector<AnimalActor*> &actors, std::vector<BioActor*> &pCollide);
	void BioActorVSBioActor(BioActor* pTest, std::vector<PlayerActor*> &actors, std::vector<BioActor*> &pCollide);


////////////////////////////////
//			PLAYERS		     //
///////////////////////////////

	bool AddNewPlayer(PlayerActor* new_player);
	/*! Removes the Player Object.*/
	bool RemovePlayerActor(const int ID);
	/*! Returns Player Objects.*/
	inline const std::vector<PlayerActor*>& GetPlayers() const {return this->zPlayers;}

////////////////////////////////
//			ANIMALS		     //
///////////////////////////////

	/*! Removes the Animal Object.*/
	bool RemoveAnimalActor(const int ID);
	/*! Returns Animals.*/
	inline const std::vector<AnimalActor*>& GetAnimals() const {return this->zAnimals;}

////////////////////////////////
//			Objects		     //
///////////////////////////////
	bool AddNewStaticFoodActor(FoodObject* new_Food);
	bool AddNewStaticWeaponActor(WeaponObject* new_Weapon);
	bool AddNewStaticContainerActor(ContainerObject* new_Container);
	bool AddNewStaticProjectileActor(StaticProjectileObject* new_Projectile);

	bool AddNewDynamicProjectileActor(DynamicProjectileObject* new_Projectile, Vector3 direction);
	/*! Removes the Static Object.*/
	bool RemoveStaticFoodActor(const int ID);
	bool RemoveStaticWeaponActor(const int ID);
	bool RemoveStaticContainerActor(const int ID);
	bool RemoveStaticProjectileActor(const int ID);
	/*! Removes the Dynamic Object.*/
	bool RemoveDynamicProjectileActor(const int ID);
	/*! Returns Food Objects.*/
	inline const std::vector<FoodObject*>& GetFoods() const {return this->zFoods;}
	/*! Returns Weapon Objects.*/
	inline const std::vector<WeaponObject*>& GetWeapons() const {return this->zWeapons;}
	/*! Returns Container Objects.*/
	inline const std::vector<ContainerObject*>& GetContainers() const {return this->zContainers;}
	/*! Returns Dynamic Projectiles Objects.*/
	inline const std::vector<DynamicProjectileObject*>& GetDynamicProjectiles() const {return this->zDynamicProjectiles;}
	/*! Returns Static Projectiles Objects.*/
	inline const std::vector<StaticProjectileObject*>& GetStaticProjectiles() const {return this->zStaticProjectiles;}
//////////////////////////////
	/*! Returns an Actor. TYPE is a enum to identify which Actor to get.
		ACTOR_TYPE_PLAYER,
		ACTOR_TYPE_ANIMAL,
		ACTOR_TYPE_STATIC_OBJECT_FOOD,
		ACTOR_TYPE_STATIC_OBJECT_WEAPON,
		ACTOR_TYPE_STATIC_OBJECT_PROJECTILE,
		ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE
	*/
	Actor* GetActor(const int ID, const int TYPE) const;
	ObjectManager* GetObjManager() const;
private:
	const int SearchForActor(const int ID, int TYPE) const;
	const int SearchForActor(const int ID, int TYPE, Actor** aOut) const;

private:

	std::vector<PlayerActor*> zPlayers;
	std::vector<AnimalActor*> zAnimals;
	std::vector<FoodObject*> zFoods;
	std::vector<WeaponObject*> zWeapons;
	std::vector<ContainerObject*> zContainers;
	std::vector<DynamicProjectileObject*> zDynamicProjectiles;
	std::vector<StaticProjectileObject*> zStaticProjectiles;
	ObjectManager* zObjManager;
	PhysicsEngine* zPhysicsEngine;

};