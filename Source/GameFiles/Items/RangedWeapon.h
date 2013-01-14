/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Weapon.h"

class RangedWeapon : public Weapon
{
public:
	RangedWeapon(const unsigned int id, const unsigned int itemType, const float damage, const float range);
	virtual ~RangedWeapon();

	virtual bool Use();

	void UseWeapon(float& range, float& damage);
private:

};