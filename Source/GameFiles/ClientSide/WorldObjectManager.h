/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/PlayerObject.h"
#include "GameFiles/ClientSide/AnimalObject.h"
#include "GameFiles/ClientSide/StaticObject.h"
#include "GameFiles/ClientSide/DynamicObject.h"
#include "iTerrain.h"
#include <vector>

static const enum OBJECT_TYPE
{
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_ANIMAL,
	OBJECT_TYPE_STATIC_OBJECT,
	OBJECT_TYPE_DYNAMIC_OBJECT
};

class WorldObjectManager
{
public:
	WorldObjectManager();
	virtual ~WorldObjectManager();

	bool AddObject(PlayerObject* player);
	bool AddObject(AnimalObject* player);
	bool AddObject(StaticObject* player);
	bool AddObject(DynamicObject* player);

	bool RemoveObject(const unsigned int type, const unsigned int id);
	bool SearchAndRemove(const unsigned int type, const unsigned int id);

	inline std::vector<PlayerObject*>& GetPlayers() {return this->zPlayerObjects;}
	inline std::vector<AnimalObject*>& GetAnimals() {return this->zAnimalObjects;}
	inline std::vector<StaticObject*>& GetStaticObjects() {return this->zStaticObjects;}
	inline std::vector<DynamicObject*>& GetDynamicObjects() {return this->zDynamicObjects;}

	PlayerObject* GetPlayerObject(const unsigned int pos);
	AnimalObject* GetAnimalObject(const unsigned int pos);
	StaticObject* GetStaticObject(const unsigned int pos);
	DynamicObject* GetDynamicObject(const unsigned int pos);

	/*! Search for the Object With the ID.*/
	int SearchForObject(const unsigned int type, const unsigned int id);

	PlayerObject* SearchAndGetPlayerObject(const unsigned int id);
	AnimalObject* SearchAndGetAnimalObject(const unsigned int id);
	StaticObject* SearchAndGetStaticObject(const unsigned int id);
	DynamicObject* SearchAndGetDynamicObject(const unsigned int id);
	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(float deltaTime);

private:
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForPlayerObject(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForAnimalObject(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForStaticObject(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForDynamicObject(const unsigned int id);

private:
	/*! Vectors to keep track of World Objects.*/
	std::vector<PlayerObject*> zPlayerObjects;
	std::vector<AnimalObject*> zAnimalObjects;
	std::vector<StaticObject*> zStaticObjects;
	std::vector<DynamicObject*> zDynamicObjects;
};