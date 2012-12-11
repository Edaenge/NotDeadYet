/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Inventory/Weapon.h"

class RangedWeapon : public Weapon
{
public:
	RangedWeapon(unsigned int id);
	RangedWeapon(unsigned int id, float damage, float range);
	virtual ~RangedWeapon();

	void UseWeapon(float& range, float& damage);
private:

};