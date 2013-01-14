
#include "Projectile.h"

Projectile::Projectile(const unsigned int id, const unsigned int itemType, float velocity, float damage) 
		   : Item(id, itemType)
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