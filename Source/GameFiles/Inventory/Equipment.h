/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Inventory/RangedWeapon.h"
#include "GameFiles/Inventory/MeleeWeapon.h"

class Equipment
{
public:
	Equipment();
	virtual ~Equipment();

	void ChangeWeapon(Weapon* weapon);
	void UseWeapon();
private:
	Weapon* zActiveWeapon;
};