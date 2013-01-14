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

enum EQUIPMENT_SLOT
{
	EQUIPMENT_SLOT_AMMO,
	EQUIPMENT_SLOT_WEAPON,
	EQUIPMENT_SLOT_HEAD,
	EQUIPMENT_SLOT_CHEST,
	EQUIPMENT_SLOT_LEGS,
	EQUIPMENT_SLOT_BOOTS
};

class Equipment
{
public:
	Equipment();
	virtual ~Equipment();
	/*! Equips Weapon*/
	void EquipWeapon(Weapon* weapon);
	void EquipProjectile(Projectile* projectile);
	void UnEquipProjectile();
	Projectile* GetProjectile();
	Weapon* GetWeapon();
	void UnEquipWeapon();
	bool EquipGear(const unsigned int type, Gear* item);
	Gear* GetGear(const unsigned int type);
	void UnEquipGear(const unsigned int type);
private:
	Projectile* zProjectile;
	Weapon* zWeapon;
	std::vector<Gear*> zGear;
};