#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(const unsigned int id,const unsigned int itemType) : Weapon(id, itemType)
{

}

RangedWeapon::RangedWeapon(const unsigned int id, const unsigned int weight, const std::string& name, const unsigned int itemType, 
						   const std::string& description, const float damage, const float range)
						   : Weapon(id, weight, name, itemType, description, damage, range)
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