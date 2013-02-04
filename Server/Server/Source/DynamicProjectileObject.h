/*
	Made by Jensen Christopher Datum(11/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "DynamicObjectActor.h"

class DynamicProjectileObject : public DynamicObjectActor
{
public:
	DynamicProjectileObject(const bool genID = true);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID if GenId = True.*/
	DynamicProjectileObject(const DynamicProjectileObject& other, bool genID = false);
	DynamicProjectileObject(const DynamicProjectileObject* other, bool genID = false);
	//DynamicProjectileObject(const StaticProjectileObject* other, bool genID = false);
	/*! */
	virtual ~DynamicProjectileObject();

	float GetSpeed() const {return this->zSpeed;}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}

	void SetSpeed(const float speed){this->zSpeed = speed;}
	void SetDamage(const float damage) {this->zDamage = damage;}
	virtual void Update(float deltaTime);

private:
	float zSpeed;
	float zDamage;
};