/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Weapon.h"

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(unsigned int id);
	MeleeWeapon(unsigned int id, float damage, float range);
	MeleeWeapon(const unsigned int id, const unsigned int weight, const std::string& name, 
		const float damage, const float range);
	virtual ~MeleeWeapon();

	void Use();
	void UseWeapon(float& range, float& damage);
private:
	
};