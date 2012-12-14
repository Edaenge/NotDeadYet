/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "GameFiles/Items/Weapon.h"

class RangedWeapon : public Weapon
{
public:
	RangedWeapon(unsigned int id);
	RangedWeapon(unsigned int id, float damage, float range);
	RangedWeapon(const unsigned int id, const unsigned int weight, const std::string& name, 
		const float damage, const float range);
	virtual ~RangedWeapon();

	void Use();

	void UseWeapon(float& range, float& damage);
private:

};