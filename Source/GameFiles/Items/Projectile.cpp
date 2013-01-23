#include "Projectile.h"

Projectile::Projectile(const long id, const unsigned int itemType, float velocity, float damage) 
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
	if (this->zStacks > 0)
	{
		this->zStacks -= 1;
		return true;
	}
	return false;
}