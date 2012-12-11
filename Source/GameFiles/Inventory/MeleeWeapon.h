/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Inventory/Weapon.h"

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(unsigned int id);
	MeleeWeapon(unsigned int id, float damage, float range);
	virtual ~MeleeWeapon();

	void UseWeapon(float& range, float& damage);
private:
	
};