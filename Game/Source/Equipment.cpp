#include "Equipment.h"
#include "ClientServerMessages.h""
#include <Safe.h>

const unsigned int GEAR_SLOTS = 4;

Equipment::Equipment()
{
	this->zRangedWeapon = NULL;
	this->zMeleeWeapon = NULL;
	this->zProjectile = NULL;
	for (unsigned int i = 0; i < GEAR_SLOTS; i++)
	{
		this->zGear.push_back(NULL);
	}
}

Equipment::~Equipment()
{
	SAFE_DELETE(this->zRangedWeapon);
	SAFE_DELETE(this->zMeleeWeapon);
	SAFE_DELETE(this->zProjectile);

	for (auto x = this->zGear.begin(); x < this->zGear.end(); x++)
	{
		SAFE_DELETE((*x));
	}
}

void Equipment::EquipRangedWeapon(RangedWeapon* weapon)
{
	this->zRangedWeapon = weapon;
}

void Equipment::EquipMeleeWeapon(MeleeWeapon* weapon)
{
	this->zMeleeWeapon = weapon;
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

MeleeWeapon* Equipment::GetMeleeWeapon()
{
	return this->zMeleeWeapon;
}

RangedWeapon* Equipment::GetRangedWeapon()
{
	return this->zRangedWeapon;
}

void Equipment::UnEquipRangedWeapon()
{
	if (Messages::FileWrite())
		Messages::Debug("UnEquipped Weapon");

	this->zRangedWeapon = NULL;
}

void Equipment::UnEquipMeleeWeapon()
{
	if (Messages::FileWrite())
		Messages::Debug("UnEquipped Weapon");

	this->zMeleeWeapon = NULL;
}

void Equipment::EquipProjectile(Projectile* projectile)
{
	if (!projectile)
		return;

	if (this->zProjectile)
	{
		if (Messages::FileWrite())
			Messages::Debug("Equipped projectile");

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
	if (Messages::FileWrite())
		Messages::Debug("UnEquipped Projectile");

	this->zProjectile = NULL;
}
Projectile* Equipment::GetProjectile()
{
	return this->zProjectile;
}