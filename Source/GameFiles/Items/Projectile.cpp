
#include "Projectile.h"

Projectile::Projectile(const unsigned int id, const unsigned int weight, const std::string& name, 
		   const unsigned int itemType, const std::string& description, float velocity, float damage) 
		   : Item(id, weight, name, itemType, description)
{
	this->zStacking = true;
	this->zDamage = damage;
	this->zVelocity = velocity;
}

Projectile::~Projectile()
{

}

bool Projectile::Use()
{
	return true;
}