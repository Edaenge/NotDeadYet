#include "RangedWeapon.h"

RangedWeapon::RangedWeapon() : Weapon()
{
	this->zStacking = false;
	this->zStacks = 0;
}

RangedWeapon::RangedWeapon(const unsigned int ID, const unsigned int itemType, const int itemSubType, 
						   const float damage, const float range) 
						   : Weapon(ID, itemType, itemSubType, damage, range)
{
	this->zStacking = false;
	this->zStacks = 0;
}


RangedWeapon::RangedWeapon(const unsigned int itemType, const int itemSubType, 
						   const float damage, const float range) 
						   : Weapon(itemType, itemSubType, damage, range)
{
	this->zStacking = false;
	this->zStacks = 0;
}

RangedWeapon::~RangedWeapon()
{

}

RangedWeapon::RangedWeapon(const RangedWeapon& other)
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
	this->zStacking = other.zStacking;
	this->zSlotSize = other.zSlotSize;
	this->zMeshModel = other.zMeshModel;
	this->zItemSubType = other.zItemSubType;
	this->zItemDescription = other.zItemDescription;
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