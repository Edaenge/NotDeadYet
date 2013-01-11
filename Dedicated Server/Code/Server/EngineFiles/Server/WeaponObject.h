/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "StaticObjectActor.h"

/**/
class WeaponObject : public StaticObjectActor
{
public:
	WeaponObject(const bool genID = false);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	WeaponObject( const WeaponObject* other, const bool genID = false);
	WeaponObject( const WeaponObject& other, const bool genID = false);
	/*! */
	virtual ~WeaponObject();

	float GetDamage() const {return this->zDamage;}
	float GetRange() const {return this->zRange;}

	void SetDamage(const float dmg){this->zDamage = dmg;}
	void SetRange (const float range){this->zRange = range;}
	
private:
	float zDamage;
	float zRange;
};