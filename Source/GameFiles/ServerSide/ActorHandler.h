/*
Made by Edänge Simon 2012-12-17
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/PlayerActor.h"
#include "GameFiles/ServerSide/WeaponObject.h"
#include "GameFiles/ServerSide/FoodObject.h"
#include "GameFiles/ServerSide/AnimalActor.h"

static const enum ACTOR_TYPE
{
	ACTOR_TYPE_PLAYER,
	ACTOR_TYPE_ANIMAL,
	ACTOR_TYPE_STATIC_OBJECT_FOOD,
	ACTOR_TYPE_STATIC_OBJECT_WEAPON,
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
	/*! Removes the player and returns the Actor for manually deallocation.*/
	PlayerActor* RemovePlayerActor(const int ID);
	/*! Returns Players.*/
	inline const std::vector<PlayerActor*>& GetPlayers() const {return this->zPlayers;}

////////////////////////////////
//			ANIMALS		     //
///////////////////////////////

	/*! Updates the animals*/
	void UpdateA(float deltaTime);

//////////////////////////////
	/*! Returns an Actor. TYPE is a enum to identify which Actor to get.
		ACTOR_TYPE_PLAYER,
		ACTOR_TYPE_ANIMAL,
		ACTOR_TYPE_STATIC_OBJECT,
		ACTOR_TYPE_DYNAMIC_OBJECT
	*/
	Actor* GetActor(const int ID, const int TYPE) const;

private:
	const unsigned int SearchForActor(const int ID, int TYPE) const;
	const unsigned int SearchForActor(const int ID, int TYPE, Actor** aOut) const;

private:

	std::vector<PlayerActor *> zPlayers;
	std::vector<AnimalActor *> zAnimals;
	std::vector<FoodObject *> zFoods;
	std::vector<WeaponObject *> zWeapons;

};