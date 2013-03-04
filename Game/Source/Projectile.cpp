#include "Projectile.h"

Projectile::Projectile() : Item()
{
	this->zStacking = true;
	this->zDamage = 0;
	this->zVelocity = 0;
}

Projectile::Projectile(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType, 
					   float velocity, float damage )
					   : Item(ID, itemType, itemSubType)
{
	this->zStacking = true;
	this->zDamage = damage;
	this->zVelocity = velocity;
}

Projectile::Projectile(const unsigned int itemType, const unsigned int itemSubType, float velocity, float damage)
	: Item(itemType, itemSubType)
{
	this->zStacking = true;
	this->zDamage = damage;
	this->zVelocity = velocity;
}

Projectile::~Projectile()
{

}

Projectile::Projectile(const Projectile& other)
{
	if (other.zID == 0)
		this->GenerateID();
	else
		this->zID = other.zID;

	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zDamage = other.zDamage;
	this->zVelocity = other.zVelocity;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zStacking = other.zStacking;
	this->zSlotSize = other.zSlotSize;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
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

std::string Projectile::ToMessageString( NetworkMessageConverter* NMC )
{
	std::string msg = Item::ToMessageString(NMC);

	msg += NMC->Convert(MESSAGE_TYPE_PROJECTILE_DAMAGE, (float)this->zDamage);
	msg += NMC->Convert(MESSAGE_TYPE_PROJECTILE_VELOCITY, (float)this->zVelocity);

	return msg;
}