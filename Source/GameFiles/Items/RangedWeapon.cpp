#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(const unsigned int id, const unsigned int itemType, const float damage, const float range)
						   : Weapon(id, itemType, damage, range)
{
	this->zStacking = false;
}
RangedWeapon::~RangedWeapon()
{

}

void RangedWeapon::UseWeapon(float& range, float& damage)
{
	range = this->zRange;
	damage = this->zDamage;
}

bool RangedWeapon::Use()
{
	return true;
}