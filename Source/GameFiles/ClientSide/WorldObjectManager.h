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

	inline const std::vector<PlayerObject*>& GetPlayers() {return this->zPlayers;}
	inline const std::vector<AnimalObject*>& GetAnimals() {return this->zAnimals;}
	inline const std::vector<StaticObject*>& GetStaticObjects() {return this->zStaticObjects;}
	inline const std::vector<DynamicObject*>& GetDynamicObjects() {return this->zDynamicObjects;}

	PlayerObject* GetPlayerObject(const unsigned int pos);
	AnimalObject* GetAnimalObject(const unsigned int pos);
	StaticObject* GetStaticObject(const unsigned int pos);
	DynamicObject* GetDynamicObject(const unsigned int pos);

	/*! Search for the Object With the ID.*/
	int SearchForObject(const unsigned int type, const unsigned int id);
	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(float deltaTime);

	bool AddMapObject(iMesh* object);
	bool AddTerrain(iTerrain* terrain);

	iMesh* GetMapObject(unsigned int position);
	iTerrain* GetTerrain();
private:
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForPlayer(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForAnimal(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForStaticObject(const unsigned int id);
	/*! Search for the object with the correct ID and returns a Position if found.*/
	int SearchForDynamicObject(const unsigned int id);

private:
	/*! Vectors to keep track of World Objects.*/
	std::vector<PlayerObject*> zPlayers;
	std::vector<AnimalObject*> zAnimals;
	std::vector<StaticObject*> zStaticObjects;
	std::vector<DynamicObject*> zDynamicObjects;
	/*! Objects that are part part of the map are stored here
	for example trees.*/
	std::vector<iMesh*> zMapObjects;
	iTerrain* zTerrain;
};