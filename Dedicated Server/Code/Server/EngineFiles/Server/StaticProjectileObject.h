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

	float GetVelocity() const {return this->zVelocity;}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}

	void SetVelocity(const float velocity){this->zVelocity = velocity;}
	
	void SetDamage(const float damage) {this->zDamage = damage;}

private:
	float zVelocity;
	float zDamage;
};