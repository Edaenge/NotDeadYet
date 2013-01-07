/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/WorldObject.h"

static const enum INTERPOLATION_TYPES
{
	IT_LINEAR,
	IT_COSINE,
	IT_ACCELERATION,
	IT_SMOOTH_STEP,
	IT_DECELERATION
};

/* Abstract Class With data for Moving Objects*/
class MovingObject : public WorldObject
{
public:
	MovingObject(const unsigned int id);
	virtual ~MovingObject() {}
	/*! Sets the State*/
	inline void SetState(const int state) {this->zState = state;}
	/*! Returns Current State*/
	inline int GetState() const {return this->zState;}
	/*! Sets the New Position*/
	inline void SetNextPosition(const Vector3 position) {this->zEndPosition = position;}
	/*! Returns New Position*/
	Vector3 GetNextPosition() const {return this->zEndPosition;}

	virtual void Update(float dt) = 0;

protected:
	/*! Interpolates the position*/
	void LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float t)
	{ CurrPos = CurrPos + (newPos - CurrPos) * t * zVelocity;}
	float GetInterpolationType(const float deltaTime, const unsigned int type);
protected:
	int zState;
	float zVelocity;
	Vector3 zEndPosition;
};