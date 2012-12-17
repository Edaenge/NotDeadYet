#include "GameFiles/Items/MeleeWeapon.h"

MeleeWeapon::MeleeWeapon(const unsigned int id,const unsigned int itemType) : Weapon(id, itemType)
{

}

MeleeWeapon::MeleeWeapon(const unsigned int id, const unsigned int weight, const std::string& name, 
						 const unsigned int itemType, const float damage, const float range) 
						 : Weapon(id, weight, name, itemType, damage, range)
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

