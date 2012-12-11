#include "GameFiles/Inventory/Equipment.h"

Equipment::Equipment()
{
	this->zActiveWeapon = 0;
}

Equipment::~Equipment()
{
	delete this->zActiveWeapon;
}

void Equipment::ChangeWeapon(Weapon* weapon)
{
	this->zActiveWeapon = weapon;
}

void Equipment::UseWeapon()
{
	if (this->zActiveWeapon)
	{
		//this->zActiveWeapon->UseWeapon();
	}
}
