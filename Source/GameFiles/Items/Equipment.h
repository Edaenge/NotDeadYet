/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/RangedWeapon.h"
#include "GameFiles/Items/MeleeWeapon.h"
#include "GameFiles/Items/Gear.h"
#include <vector>

enum GEAR_TYPE
{
	GEAR_HEAD,
	GEAR_CHEST,
	GEAR_LEGS,
	GEAR_BOOTS
};

class Equipment
{
public:
	Equipment();
	virtual ~Equipment();

	void EquipWeapon(Weapon* weapon);
	Weapon* GetWeapon();
	void UnEquipWeapon();
	void UseWeapon();
	bool EquipGear(const unsigned int type, Gear* item);
	Gear* GetGear(const unsigned int type);
	void UnEquipGear(const unsigned int type);
private:
	Weapon* zWeapon;
	std::vector<Gear*> zGear;
};