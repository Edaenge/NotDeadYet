/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "Actor.h"
#include <vector>

class Updates
{
public:
	Updates(const unsigned int ID) 
	{
		this->zID = ID;
		this->zStateChange = false;
		this->zPositionChange = false;
		this->zRotationChange = false;
	}

	unsigned int GetID() const {return this->zID;}
	Vector3 GetPosition() const {return this->zNextPosition;}
	Vector4 GetRotation() const {return this->zRotation;}
	unsigned int GetState() const {return this->zState;}

	void SetPosition(Vector3 position) 
	{
		this->zNextPosition = position;
		this->zPositionChange = true;
	}
	void SetRotation(Vector4 rotation) 
	{
		this->zRotation = rotation;
		this->zRotationChange = true;
	}
	void SetState(unsigned int state) 
	{
		this->zState = state;
		this->zStateChange = true;
	}
	bool ComparePosition(const Vector3& position)
	{
		if ((this->zNextPosition - position).GetLength() < 0.5f)
			this->zPositionChange = false;

		return this->zPositionChange;
	}
	bool CompareRotation(const Vector4& rotation)
	{
		if ((this->zRotation - rotation).GetLength() < 0.5f)
			this->zRotationChange = false;

		return this->zRotationChange;
	}
	bool CompareState(const float state)
	{
		if (this->zState == state)
			this->zStateChange = false;

		return this->zStateChange;
	}
private:
	unsigned int zID;
	Vector3 zNextPosition;
	Vector4 zRotation;
	unsigned int zState;

	bool zStateChange;
	bool zPositionChange;
	bool zRotationChange;
};

class WorldObjectManager
{
public:
	WorldObjectManager();
	virtual ~WorldObjectManager();
	/*
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
	//! Search for the Object With the ID.
	int SearchForObject(const unsigned int type, const int ID);
	
	PlayerObject* SearchAndGetPlayerObject(const int ID);
	AnimalObject* SearchAndGetAnimalObject(const long ID);
	StaticObject* SearchAndGetStaticObject(const long ID);
	DynamicObject* SearchAndGetDynamicObject(const long ID);
	DeadPlayerObject* SearchAndGetDeadPlayerObject(const long ID);
	*/
	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(float deltaTime);

	bool AddActor(Actor* actor);
	Actor* SearchAndGetActor(const long ID);
	Actor* GetActor(const int Index);
	bool RemoveActor(const int Index);
	int SearchForActor(const long ID);
	bool AddUpdate(Updates update);
private:
	std::vector<Actor*> zActors;
	std::vector<Updates> zUpdates;
};