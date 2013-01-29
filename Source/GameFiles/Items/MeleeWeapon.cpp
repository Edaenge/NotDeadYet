#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon(const long id, const unsigned int itemType, const float damage, const float range)
	: Weapon(id, itemType, damage, range)
{
	this->zStacking = false;
}

MeleeWeapon::MeleeWeapon(const MeleeWeapon& other)
{
	this->zID = other.zID;
	this->zRange = other.zRange;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zDamage = other.zDamage;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zItemDescription = other.zItemDescription;
}

MeleeWeapon::MeleeWeapon(const MeleeWeapon* other)
{
	this->zID = other->zID;
	this->zRange = other->zRange;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zDamage = other->zDamage;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zItemDescription = other->zItemDescription;
}

MeleeWeapon::~MeleeWeapon()
{

}

void MeleeWeapon::UseWeapon(float& range, float& damage)
{
	range = this->zRange;
	damage = this->zDamage;
}

bool MeleeWeapon::Use()
{

	return true;
}
