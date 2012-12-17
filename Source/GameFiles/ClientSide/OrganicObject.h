/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/WorldObject.h"

class OrganicObject : public WorldObject
{
public:
	OrganicObject(const unsigned int id) : WorldObject(id)
	{
		this->zState = STATE_IDLE; 
		this->zEndPosition = Vector3(0,0,0);
		this->zVelocity = 500.0f;
	}
	virtual ~OrganicObject() {}
	/*! Sets the State*/
	inline void SetState(const int state) {this->zState = state;}
	/*! Returns Current State*/
	inline int GetState() const {return this->zState;}
	/*! Sets the New Position*/
	inline void SetNextPosition(const Vector3 position) {this->zEndPosition = position;}
	/*! Returns New Position*/
	Vector3 GetNextPosition() const {return this->zEndPosition;}
	/*! Return the Health*/
	inline float GetHealth() const {return this->zHealth;}
	/*! Sets the Health*/
	inline void SetHealth(const float health) {this->zHealth = health;}

	virtual void Update(float dt) = 0;

protected:
	/*! Interpolates the position*/
	void LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float t)
	{ 
		CurrPos = CurrPos + (newPos - CurrPos) * t * zVelocity;
	}
protected:
	int zState;
	float zHealth;
	float zVelocity;
	Vector3 zEndPosition;
};