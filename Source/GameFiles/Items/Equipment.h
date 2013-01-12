/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "Gear.h"
#include "Projectile.h"

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
	/*! Equips Weapon*/
	void EquipWeapon(Weapon* weapon);
	void EquipAmmo(Projectile* projectile);
	void UnEquipAmmo();
	Projectile* GetAmmo();
	Weapon* GetWeapon();
	void UnEquipWeapon();
	bool EquipGear(const unsigned int type, Gear* item);
	Gear* GetGear(const unsigned int type);
	void UnEquipGear(const unsigned int type);
private:
	Projectile* zAmmo;
	Weapon* zWeapon;
	std::vector<Gear*> zGear;
};