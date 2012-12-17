/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Weapon.h"

class RangedWeapon : public Weapon
{
public:
	RangedWeapon(const unsigned int id,const unsigned int itemType);
	RangedWeapon(const unsigned int id, const unsigned int weight, const std::string& name, const unsigned int itemType,
		const float damage, const float range);
	virtual ~RangedWeapon();

	void Use();

	void UseWeapon(float& range, float& damage);
private:

};