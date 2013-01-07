/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Weapon.h"

class MeleeWeapon : public Weapon
{
public:
	MeleeWeapon(const unsigned int id,const unsigned int itemType);
	MeleeWeapon(const unsigned int id, const unsigned int weight, const std::string& name, const unsigned int itemType, 
		const std::string& description, const float damage, const float range);
	virtual ~MeleeWeapon();

	virtual void Use();
	void UseWeapon(float& range, float& damage);
private:
	
};