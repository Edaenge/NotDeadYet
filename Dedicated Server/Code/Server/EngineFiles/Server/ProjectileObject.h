/*
	Made by Jensen Christopher Datum(11/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "DynamicObjectActor.h"

class ProjectileObject : public DynamicObjectActor
{
public:
	ProjectileObject(const bool genID = true);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	ProjectileObject(const ProjectileObject& other);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	ProjectileObject& operator= (const ProjectileObject& other);
	/*! */
	virtual ~ProjectileObject();

	float GetVelocity() const {return this->zVelocity;}
	/*! Returns number of stacks the item has.*/
	unsigned int GetStackSize() const {return this->zStacks;}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}

	void SetVelocity(const float velocity){this->zVelocity = velocity;}
	void ModifyStackSize(const unsigned int size) {this->zStacks += size;}
	void SetDamage(const float damage) {this->zDamage = damage;}

	virtual void Update(float deltaTime);

private:
	float zVelocity;
	int zStacks;
	float zDamage;
};