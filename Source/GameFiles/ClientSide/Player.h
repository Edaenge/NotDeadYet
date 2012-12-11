/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/WorldObject.h"

//Keeps track of player Info
class Player : public WorldObject
{
public:
	Player();
	virtual ~Player(){}
	/*! Sets the Player State*/
	inline void SetPlayerState(const int state) {this->zPlayerState = state;}
	/*! Returns Current Player State*/
	inline int GetPlayerState() const {return this->zPlayerState;}
	/*! Sets the New Player Position*/
	inline void SetNextPosition(const Vector3 position) {this->zEndPosition = position;}
	/*! Returns New Player Position*/
	Vector3 GetNextPosition() const {return this->zEndPosition;}
	/*! Updates the player*/
	void Update(float deltaTime);

private:
	void LinearInterpolation(Vector3& PrevPos, const Vector3& newPos, float dt);

private:
	int zPlayerState;
	Vector3 zEndPosition;
	float zVelocity;
};