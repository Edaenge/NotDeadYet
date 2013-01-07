/*
Made by Edänge Simon 2012-12-17
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/PlayerActor.h"
#include "GameFiles/ServerSide/AnimalActor.h"
#include "GameFiles/ServerSide/ObjectManager.h"

static const enum ACTOR_TYPE
{
	ACTOR_TYPE_PLAYER,
	ACTOR_TYPE_ANIMAL,
	ACTOR_TYPE_STATIC_OBJECT_FOOD,
	ACTOR_TYPE_STATIC_OBJECT_WEAPON,
	ACTOR_TYPE_STATIC_OBJECT_CONTAINER,
	ACTOR_TYPE_DYNAMIC_OBJECT
};

class ActorHandler
{

public:
	ActorHandler();
	virtual ~ActorHandler();


////////////////////////////////
//			PLAYERS		     //
///////////////////////////////

	/*! Updates the players.*/
	void UpdatePl(float deltaTime);
	bool AddNewPlayer(PlayerActor* new_player);
	/*! Removes the Player Object.*/
	bool RemovePlayerActor(const int ID);
	/*! Returns Player Objects.*/
	inline const std::vector<PlayerActor*>& GetPlayers() const {return this->zPlayers;}

////////////////////////////////
//			ANIMALS		     //
///////////////////////////////

	/*! Updates the animals*/
	void UpdateA(float deltaTime);
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
	/*! Removes the Static Object.*/
	bool RemoveStaticFoodActor(const int ID);
	bool RemoveStaticWeaponActor(const int ID);
	bool RemoveStaticContainerActor(const int ID);
	/*! Removes the Dynamic Object.*/
	bool RemoveDynamicActor(const int ID);
	/*! Returns Food Objects.*/
	inline const std::vector<FoodObject*>& GetFoods() const {return this->zFoods;}
	/*! Returns Weapon Objects.*/
	inline const std::vector<WeaponObject*>& GetWeapons() const {return this->zWeapons;}
	/*! Returns Container Objects.*/
	inline const std::vector<ContainerObject*>& GetContainers() const {return this->zContainer;}

//////////////////////////////
	/*! Returns an Actor. TYPE is a enum to identify which Actor to get.
		ACTOR_TYPE_PLAYER,
		ACTOR_TYPE_ANIMAL,
		ACTOR_TYPE_STATIC_OBJECT_FOOD,
		ACTOR_TYPE_STATIC_OBJECT_WEAPON,
		ACTOR_TYPE_DYNAMIC_OBJECT
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
	std::vector<ContainerObject*> zContainer;

	ObjectManager* zObjManager;

};