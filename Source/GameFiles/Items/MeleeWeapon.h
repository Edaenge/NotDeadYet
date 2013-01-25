/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Weapon.h"

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(const long id, const unsigned int itemType, const float damage, const float range);
	MeleeWeapon(const MeleeWeapon& other);
	MeleeWeapon(const MeleeWeapon* other);
	virtual ~MeleeWeapon();

	virtual bool Use();
	void UseWeapon(float& range, float& damage);
private:
	
};