#include "Equipment.h"

const unsigned int GEAR_SLOTS = 4;

Equipment::Equipment()
{
	this->zWeapon = 0;
	for (unsigned int i = 0; i < GEAR_SLOTS; i++)
	{
		this->zGear.push_back(NULL);
	}
	this->zProjectile = NULL;
}

Equipment::~Equipment()
{
	if (this->zWeapon)
	{
		delete this->zWeapon;
		this->zWeapon = NULL;
	}
	for (auto x = this->zGear.begin(); x < this->zGear.end(); x++)
	{
		if ((*x))
		{
			delete (*x);
			(*x) = NULL;
		}
	}
}

void Equipment::EquipWeapon(Weapon* weapon)
{
	this->zWeapon = weapon;
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
	return NULL;
}

void Equipment::UnEquipGear(const unsigned int type)
{
	if (type < GEAR_SLOTS)
	{
		this->zGear[type] = NULL;
	}
}

Weapon* Equipment::GetWeapon()
{
	return this->zWeapon;
}

void Equipment::UnEquipWeapon()
{
	this->zWeapon = NULL;
}

void Equipment::EquipProjectile(Projectile* projectile)
{
	if (!projectile)
		return;

	if (this->zProjectile)
	{
		if (this->zProjectile->GetItemType() == projectile->GetItemType())
		{
			int totalStacks = this->zProjectile->GetStackSize() + projectile->GetStackSize();
			this->zProjectile->SetStackSize(totalStacks);
		}
		else
		{
			this->zProjectile = projectile;
		}
	}
	else
	{
		this->zProjectile = projectile;
	}
	
}

void Equipment::UnEquipProjectile()
{
	this->zProjectile = NULL;
}
Projectile* Equipment::GetProjectile()
{
	return this->zProjectile;
}