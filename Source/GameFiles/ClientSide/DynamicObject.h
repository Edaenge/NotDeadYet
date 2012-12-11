/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/WorldObject.h"

/*! Class to Handle Dynamic Objects*/
class DynamicObject : public WorldObject
{
public:
	DynamicObject();
	virtual ~DynamicObject(){}
	/*! Sets the State*/
	inline void SetState(const int state) {this->zState = state;}
	/*! Returns Current State*/
	inline int GetState() const {return this->zState;}
	/*! Sets the New Position*/
	inline void SetNextPosition(const Vector3 position) {this->zEndPosition = position;}
	/*! Returns New Position*/
	Vector3 GetNextPosition() const {return this->zEndPosition;}
	/*! Updates the Animal*/
	void Update(float deltaTime);

private:
	/*! Interpolates the position*/
	void LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float dt);

private:
	int zState;
	Vector3 zEndPosition;
	float zVelocity;
};