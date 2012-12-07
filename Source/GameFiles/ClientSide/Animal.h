/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/WorldObject.h"
#include "GameFiles/AnimationStates.h"

/*! Class to Handle Animals*/
class Animal : public WorldObject
{
public:
	Animal();
	virtual ~Animal(){}
	/*! Sets the Animal State*/
	inline void SetAnimalState(const int state) {this->zAnimalState = state;}
	/*! Returns Current Animal State*/
	inline int GetAnimalState() const {return this->zAnimalState;}
	/*! Sets the New Animal Position*/
	inline void SetNextPosition(const Vector3 position) {this->zEndPosition = position;}
	/*! Returns New Animal Position*/
	Vector3 GetNextPosition() const {return this->zEndPosition;}
	/*! Updates the Animal data*/
	void Update(float deltaTime);
private:
	void LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float dt);
private:
	int zAnimalState;
	Vector3 zEndPosition;
	float zVelocity;
};
