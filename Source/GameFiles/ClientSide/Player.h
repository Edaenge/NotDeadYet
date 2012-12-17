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
	/*! Sets the State*/
	inline void SetState(const int state) {this->zState = state;}
	/*! Returns Current State*/
	inline int GetState() const {return this->zState;}
	/*! Sets the New Position*/
	inline void SetNextPosition(const Vector3 position) {this->zEndPosition = position;}
	/*! Returns New Position*/
	Vector3 GetNextPosition() const {return this->zEndPosition;}

	inline float GetHealth() const {return this->zHealth;}
	inline float GetHunger() const {return this->zHunger;}
	inline float GetHydration() const {return this->zHydration;}

	inline void SetHealth(const float health) {this->zHealth = health;}
	inline void SetHunger(const float hunger) {this->zHunger = hunger;}
	inline void SetHydration(const float hydration) {this->zHydration = hydration;}
	/*! Updates the Player*/
	virtual void Update(float deltaTime);

private:
	/*! Interpolates the position*/
	void LinearInterpolation(Vector3& PrevPos, const Vector3& newPos, float dt);

private:
	int zState;
	float zHealth;
	float zHunger;
	float zVelocity;
	float zHydration;
	Vector3 zEndPosition;
};