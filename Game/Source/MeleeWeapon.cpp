#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon() : Weapon()
{

}

MeleeWeapon::MeleeWeapon( const unsigned int ID, const unsigned int itemType, const int itemSubType, const float damage, const float range )
	: Weapon(ID, itemType, itemSubType, damage, range)
{

}

MeleeWeapon::MeleeWeapon(const unsigned int itemType, const int itemSubType, 
						 const float damage, const float range) 
						 : Weapon(itemType, itemSubType, damage, range)
{

}

MeleeWeapon::~MeleeWeapon()
{

}

MeleeWeapon::MeleeWeapon(const MeleeWeapon& other)
{
	if (other.zID == 0)
		this->GenerateID();
	else
		this->zID = other.zID;

	this->zRange = other.zRange;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zDamage = other.zDamage;
	this->zItemName = other.zItemName;
	this->zIconPath = other.zIconPath;
	this->zItemType = other.zItemType;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
}

MeleeWeapon::MeleeWeapon(const MeleeWeapon* other)
{
	this->zID = other->zID;
	this->zRange = other->zRange;
	this->zStacks = other->zStacks;
	this->zWeight = other->zWeight;
	this->zDamage = other->zDamage;
	this->zItemName = other->zItemName;
	this->zIconPath = other->zIconPath;
	this->zItemType = other->zItemType;
	this->zMeshModel = other->zMeshModel;
	this->zItemSubType = other->zItemSubType;
	this->zItemDescription = other->zItemDescription;
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
