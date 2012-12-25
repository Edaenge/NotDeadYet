/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/WorldObject.h"

class MovingObject : public WorldObject
{
public:
	MovingObject(const unsigned int id) : WorldObject(id)
	{
		this->zState = STATE_IDLE; 
		this->zEndPosition = Vector3(0,0,0);
		this->zVelocity = 500.0f;
	}
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
protected:
	int zState;
	float zVelocity;
	Vector3 zEndPosition;
};