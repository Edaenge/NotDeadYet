/*
Made by Edänge Simon Datum(17/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/StaticObjectActor.h"
#include "GameFiles/ServerSide/ObjectManager.h"

/**/
class WeaponObject : public StaticObjectActor
{
public:
	WeaponObject();
	/*! Use ObjectManager to Get Data*/
	WeaponObject( const WeaponData* data);
	virtual ~WeaponObject();

	float GetDamage() const {return this->zDamage;}
	float GetRange() const {return this->zRange;}

	void SetDamage(const float dmg){this->zDamage = dmg;}
	void SetRange (const float range){this->zRange = range;}
	
private:
	float zDamage;
	float zRange;
};