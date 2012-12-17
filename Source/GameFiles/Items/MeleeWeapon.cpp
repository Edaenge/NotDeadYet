#include "GameFiles/Items/MeleeWeapon.h"

MeleeWeapon::MeleeWeapon(unsigned int id) : Weapon(id)
{

}

MeleeWeapon::MeleeWeapon(unsigned int id, float damage, float range) : Weapon(id, damage, range)
{

}

MeleeWeapon::MeleeWeapon(const unsigned int id, const unsigned int weight, const std::string& name, 
						 const float damage, const float range) : Weapon(id, weight, name, damage, range)
{

}

MeleeWeapon::~MeleeWeapon()
{

}

void MeleeWeapon::UseWeapon(float& range, float& damage)
{
	range = this->zRange;
	damage = this->zDamage;
}

void MeleeWeapon::Use()
{

}

