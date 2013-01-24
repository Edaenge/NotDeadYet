#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon(const long id, const unsigned int itemType, const float damage, const float range)
	: Weapon(id, itemType, damage, range)
{
	this->zStacking = false;
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