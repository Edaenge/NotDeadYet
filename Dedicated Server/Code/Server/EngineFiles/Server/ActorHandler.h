/*
Made by Edänge Simon 2012-12-17
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "DeerActor.h"
#include "WolfActor.h"
#include "PlayerActor.h"
#include "ObjectManager.h"
#include "DeadPlayerObjectActor.h"
#include "DynamicProjectileObject.h"
#include "Safe.h"
#include "PhysicsEngine.h"

static const enum ACTOR_TYPE
{
	ACTOR_TYPE_PLAYER,
	ACTOR_TYPE_ANIMAL,
	ACTOR_TYPE_STATIC_OBJECT_FOOD,
	ACTOR_TYPE_STATIC_OBJECT_WEAPON,
	ACTOR_TYPE_STATIC_OBJECT_CONTAINER,
	ACTOR_TYPE_STATIC_OBJECT_PROJECTILE,
	ACTOR_TYPE_STATIC_OBJECT_MATERIAL,
	ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE,
	ACTOR_TYPE_DEAD_PLAYER
};

static const enum COLLISION_EVENT_TYPE
{
	MELEE_ATTACK
};

struct CollisionEvent
{
	CollisionEvent()
	{
		actor_aggressor_ID = -1;
		actor_victim_ID = -1;
	}
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
	ActorHandler(World* worldPtr);
	virtual ~ActorHandler();

	void UpdateObjects(float deltaTime);
	/*! Check collision against PlayerActors and Animal Actors
		Used When Attacking with melee.
		Called on player use.
	*/
	CollisionEvent CheckMeeleCollision(BioActor* pActor, float range);
	/*! Checks BioActors collisions against other BioActors objects.*/
	void CheckCollisions();
	/*! Checks collision on moving objects. If it hits a BioActor, it will take damage.
		Returns CollisionEvents of dead players.
		Called on Update.
	*/
	std::vector<CollisionEvent> CheckProjectileCollisions();
	/*! pCollide lists all collisions.
		Rewinds target BioActor in actors if there was a collision.
		pTest will not be Rewinded.
	*/
	void BioActorVSBioActors(BioActor* pTest, std::vector<AnimalActor*> &actors, std::vector<BioActor*> &pCollide);
	void BioActorVSBioActors(BioActor* pTest, std::vector<PlayerActor*> &actors, std::vector<BioActor*> &pCollide);
	void DynamicActorVsBioActors(DynamicObjectActor* pTest, std::vector<PlayerActor*> &actors, std::vector<BioActor*> &pCollide);
	void DynamicActorVsBioActors(DynamicObjectActor* pTest, std::vector<AnimalActor*> &actors, std::vector<BioActor*> &pCollide);

////////////////////////////////
//			PLAYERS		     //
///////////////////////////////

	/*! Adds new actor.
		NOTE: Physic mesh will not be created in this function.
		You have to do it manually.
	*/
	bool AddNewPlayer(PlayerActor* new_player);
	/*! Removes the Player Object.*/
	bool RemovePlayerActor(const long ID);
	/*! Returns Player Objects.*/
	inline const std::vector<PlayerActor*>& GetPlayers() const {return this->zPlayers;}
	/*! Adds new actor.
		NOTE: Physic mesh will not be created in this function.
		You have to do it manually.
	*/
	bool AddNewDeadPlayer(DeadPlayerObjectActor* new_DeadPlayer);
	/*! Removes the Dead Player Object*/
	bool RemovedDeadPlayerObject(const long ID);
	inline const std::vector<DeadPlayerObjectActor*>& GetDeadPlayers() const {return this->zDeadPlayers;}

////////////////////////////////
//			ANIMALS		     //
///////////////////////////////

	/*! Adds new actor.
		NOTE: Physic mesh will not be created in this function.
		You have to do it manually.
	*/
	bool AddNewAnimalActor(AnimalActor* new_player);	/*! Removes the Animal Object.*/
	bool RemoveAnimalActor(const long ID);
	/*! Returns Animals.*/
	inline const std::vector<AnimalActor*>& GetAnimals() const {return this->zAnimals;}

////////////////////////////////
//			Objects		     //
///////////////////////////////
	bool AddNewStaticFoodActor(FoodObject* new_Food);
	bool AddNewStaticWeaponActor(WeaponObject* new_Weapon);
	bool AddNewStaticMaterialObject(MaterialObject* new_Material);
	bool AddNewStaticContainerActor(ContainerObject* new_Container);
	bool AddNewStaticProjectileActor(StaticProjectileObject* new_Projectile);
	
	/*! Adds new actor.
		NOTE: Physic mesh will not be created in this function.
		You have to do it manually.
	*/
	bool AddNewDynamicProjectileActor(DynamicProjectileObject* new_Projectile, Vector3 direction);
	/*! Removes the Static Object.*/
	bool RemoveStaticFoodActor(const long ID);
	bool RemoveStaticWeaponActor(const long ID);
	bool RemoveStaticMaterialActor(const long ID);
	bool RemoveStaticContainerActor(const long ID);
	bool RemoveStaticProjectileActor(const long ID);
	/*! Removes the Dynamic Object.*/
	bool RemoveDynamicProjectileActor(const long ID);
	/*! Returns Food Objects.*/
	inline const std::vector<FoodObject*>& GetFoods() const {return this->zFoods;}
	/*! Returns Weapon Objects.*/
	inline const std::vector<WeaponObject*>& GetWeapons() const {return this->zWeapons;}
	/*! Returns Container Objects.*/
	inline const std::vector<ContainerObject*>& GetContainers() const {return this->zContainers;}
	/*! Returns Material Objects.*/
	inline const std::vector<MaterialObject*>& GetMaterials() const {return this->zMaterials;}
	/*! Returns Dynamic Projectile Objects.*/
	inline const std::vector<DynamicProjectileObject*>& GetDynamicProjectiles() const {return this->zDynamicProjectiles;}
	/*! Returns Static Projectile Objects.*/
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
	Actor* GetActor(const long ID, const int TYPE) const;
	ObjectManager* GetObjManager() const;
	PhysicsEngine* GetPhysicEnginePtr() const {return this->zPhysicsEngine;}
private:
	const int SearchForActor(const long ID, int TYPE) const;
	const int SearchForActor(const long ID, int TYPE, Actor** aOut) const;

private:

	World* zWorld;
	ObjectManager* zObjManager;
	PhysicsEngine* zPhysicsEngine;
	std::vector<FoodObject*> zFoods;
	std::vector<PlayerActor*> zPlayers;
	std::vector<AnimalActor*> zAnimals;
	std::vector<WeaponObject*> zWeapons;
	std::vector<MaterialObject*> zMaterials;
	std::vector<ContainerObject*> zContainers;
	std::vector<DeadPlayerObjectActor*> zDeadPlayers;
	std::vector<StaticProjectileObject*> zStaticProjectiles;
	std::vector<DynamicProjectileObject*> zDynamicProjectiles;
};