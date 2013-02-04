/*
	Made by Jensen Christopher Datum(11/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "StaticObjectActor.h"
#include "DynamicProjectileObject.h"

class StaticProjectileObject : public StaticObjectActor
{
public:
	StaticProjectileObject(const bool genID = false);	
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	StaticProjectileObject(const StaticProjectileObject& other, bool genID = false);
	StaticProjectileObject(const StaticProjectileObject* other, bool genID = false);
	StaticProjectileObject(const DynamicProjectileObject* other, bool genID = false);
	/*! */
	virtual ~StaticProjectileObject();

	float GetSpeed() const {return this->zSpeed;}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}

	void SetSpeed(const float speed){this->zSpeed = speed;}
	
	void SetDamage(const float damage) {this->zDamage = damage;}

private:
	float zSpeed;
	float zDamage;
};