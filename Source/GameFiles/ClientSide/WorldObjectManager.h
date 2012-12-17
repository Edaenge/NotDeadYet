/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/PlayerObject.h"
#include "GameFiles/ClientSide/AnimalObject.h"
#include "GameFiles/ClientSide/StaticObject.h"
#include "GameFiles/ClientSide/DynamicObject.h"
#include <vector>

static const enum OBJECT_TYPE
{
	OBJECT_TYPE_PLAYER,
	OBJECT_TYPE_STATIC_OBJECT,
	OBJECT_TYPE_DYNAMIC_OBJECT,
	OBJECT_TYPE_ANIMAL
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

	std::vector<PlayerObject*> GetPlayers();
	std::vector<AnimalObject*> GetAnimals();
	std::vector<StaticObject*> GetStaticObjects();
	std::vector<DynamicObject*> GetDynamicObjects();

	PlayerObject* GetPlayer(const unsigned int pos);
	AnimalObject* GetAnimal(const unsigned int pos);
	StaticObject* GetStaticObject(const unsigned int pos);
	DynamicObject* GetDynamicObject(const unsigned int pos);

	/*! Search for the Object With the ID*/
	int SearchForObject(const unsigned int type, const unsigned int id);

	void UpdateObjects(float deltaTime);
private:
	/*! Search for the object with the correct ID and returns a Position if found*/
	int SearchForPlayer(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found*/
	int SearchForAnimal(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found*/
	int SearchForStaticObject(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found*/
	int SearchForDynamicObject(const unsigned int id);

private:
	/*! Vectors to keep track of World Objects*/
	std::vector<PlayerObject*> zPlayers;
	std::vector<AnimalObject*> zAnimals;
	std::vector<StaticObject*> zStaticObjects;
	std::vector<DynamicObject*> zDynamicObjects;
};