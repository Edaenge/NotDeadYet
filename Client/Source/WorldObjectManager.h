/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "PlayerObject.h"
#include "AnimalObject.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "DeadPlayerObject.h"
#include <vector>

static const enum OBJECT_TYPE
{
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_ANIMAL,
	OBJECT_TYPE_STATIC_OBJECT,
	OBJECT_TYPE_DYNAMIC_OBJECT,
	OBJECT_TYPE_DEAD_PLAYER,
	OBJECT_TYPE_ACTOR
};

class WorldObjectManager
{
public:
	WorldObjectManager();
	virtual ~WorldObjectManager();

	bool AddObject(PlayerObject* new_PlayerObject);
	bool AddObject(AnimalObject* new_AnimalObject);
	bool AddObject(StaticObject* new_StaticObject);
	bool AddObject(DynamicObject* new_DynamicObject);
	bool AddObject(DeadPlayerObject* new_DeadPlayerObject);
	bool RemoveObject(const unsigned int type, const int Index);
	bool SearchAndRemove(const unsigned int type, const int ID);

	inline std::vector<PlayerObject*>& GetPlayers() {return this->zPlayerObjects;}
	inline std::vector<AnimalObject*>& GetAnimals() {return this->zAnimalObjects;}
	inline std::vector<StaticObject*>& GetStaticObjects() {return this->zStaticObjects;}
	inline std::vector<DynamicObject*>& GetDynamicObjects() {return this->zDynamicObjects;}
	inline std::vector<DeadPlayerObject*>& GetDeadPlayerObjects() {return this->zDeadPlayerObjects;}

	PlayerObject* GetPlayerObject(const unsigned int Index);
	AnimalObject* GetAnimalObject(const unsigned int Index);
	StaticObject* GetStaticObject(const unsigned int Index);
	DynamicObject* GetDynamicObject(const unsigned int Index);
	DeadPlayerObject* GetDeadPlayerObject(const unsigned int Index);
	/*! Search for the Object With the ID.*/
	int SearchForObject(const unsigned int type, const int ID);
	
	PlayerObject* SearchAndGetPlayerObject(const int ID);
	AnimalObject* SearchAndGetAnimalObject(const long ID);
	StaticObject* SearchAndGetStaticObject(const long ID);
	DynamicObject* SearchAndGetDynamicObject(const long ID);
	DeadPlayerObject* SearchAndGetDeadPlayerObject(const long ID);
	
	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(float deltaTime);

	bool AddActor(WorldObject* actor);
	WorldObject* SearchAndGetActor(const long ID);
	WorldObject* GetActor(const int Index);
	bool RemoveActor(const int Index);
	int SearchForActor(const long ID);
private:
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForPlayerObject(const int ID);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForAnimalObject(const long ID);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForStaticObject(const long ID);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForDynamicObject(const long ID);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForDeadPlayerObject(const long ID);
	
private:
	/*! Vectors to keep track of World Objects.*/
	std::vector<PlayerObject*> zPlayerObjects;
	std::vector<AnimalObject*> zAnimalObjects;
	std::vector<StaticObject*> zStaticObjects;
	std::vector<DynamicObject*> zDynamicObjects;
	std::vector<DeadPlayerObject*> zDeadPlayerObjects;
	std::vector<WorldObject*> zActors;
};