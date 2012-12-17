#include "GameFiles/Items/RangedWeapon.h"

RangedWeapon::RangedWeapon( unsigned int id ) : Weapon(id)
{

}

RangedWeapon::RangedWeapon(unsigned int id, float damage, float range) : Weapon(id, damage, range)
{

}

RangedWeapon::RangedWeapon(const unsigned int id, const unsigned int weight, const std::string& name, 
						   const float damage, const float range) : Weapon(id, weight, name, damage, range)
{

}
RangedWeapon::~RangedWeapon()
{

}

void RangedWeapon::UseWeapon(float& range, float& damage)
{
	range = this->zRange;
	damage = this->zDamage;
}

void RangedWeapon::Use()
{

}

