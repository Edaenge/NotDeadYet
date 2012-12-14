#include "GameFiles/Items/Equipment.h"

const unsigned int GEAR_SLOTS = 4;

Equipment::Equipment()
{
	this->zWeapon = 0;
	for (unsigned int i = 0; i < GEAR_SLOTS; i++)
	{
		this->zGear.push_back(0);
	}
}

Equipment::~Equipment()
{
	if (this->zWeapon)
	{
		delete this->zWeapon;
		this->zWeapon = 0;
	}
}

void Equipment::EquipWeapon(Weapon* weapon)
{
	this->zWeapon = weapon;
}

void Equipment::UseWeapon()
{
	if (this->zWeapon)
	{
		//this->zActiveWeapon->UseWeapon();
	}
}

bool Equipment::EquipGear(const unsigned int type, Gear* item)
{
	if (type < GEAR_SLOTS)
	{
		this->zGear[type] = item;
		return true;
	}
	return false;
}

Gear* Equipment::GetGear(const unsigned int type)
{
	if (type < GEAR_SLOTS)
	{
		return this->zGear[type];
	}
	return 0;
}

Gear* Equipment::UnEquipGear(const unsigned int type)
{
	if (type < GEAR_SLOTS)
	{
		this->zGear[type] = 0;
		return this->zGear[type];
	}
	return 0;
}
