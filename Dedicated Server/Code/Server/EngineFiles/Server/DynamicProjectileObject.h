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

	float GetVelocity() const {return this->zVelocity;}
	/*! Returns number of stacks the item has.*/
	unsigned int GetStackSize() const {return this->zStacks;}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}

	void SetVelocity(const float velocity){this->zVelocity = velocity;}
	void ModifyStackSize(const unsigned int size) {this->zStacks += size;}
	void SetDamage(const float damage) {this->zDamage = damage;}
	bool IsMoving() const {return this->zMoving;}
	virtual void Update(float deltaTime);

private:
	float zVelocity;
	int zStacks;
	float zDamage;
	
	bool zMoving;
};