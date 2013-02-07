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

	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(float deltaTime);

	bool AddActor(Actor* actor);
	Actor* SearchAndGetActor(const unsigned int ID);
	Actor* GetActor(const int Index);
	bool RemoveActor(const int Index);
	int SearchForActor(const unsigned int ID);
	void AddUpdate(Updates update);
private:
	std::vector<Actor*> zActors;
	std::vector<Updates> zUpdates;
};