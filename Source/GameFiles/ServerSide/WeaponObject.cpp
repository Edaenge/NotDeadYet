#include "WeaponObject.h"



WeaponObject::WeaponObject(const bool genID /* = true */) : StaticObjectActor(genID)
{
	this->zWeight = 1;
	this->zRange = 1.0f;
	this->zDamage = 0.0f;
}

WeaponObject::WeaponObject( const WeaponObject& other )
{

	this->GenerateID();
	this->zDamage = other.zDamage;
	this->zRange = other.zRange;
	this->zWeight = other.zWeight;
	this->zActorModel = other.zActorModel;
	this->zType = other.zType;
	this->zActorObjectName = other.zActorObjectName;
	this->zScale = other.zScale;
	this->zRot = other.zRot;

}

WeaponObject& WeaponObject::operator= (const WeaponObject& other)
{
	if(this == &other)
		return *this;

	this->GenerateID();
	this->zDamage = other.zDamage;
	this->zRange = other.zRange;
	this->zWeight = other.zWeight;
	this->zActorModel = other.zActorModel;
	this->zType = other.zType;
	this->zActorObjectName = other.zActorObjectName;
	this->zScale = other.zScale;
	this->zRot = other.zRot;

	return *this;
}

WeaponObject::~WeaponObject()
{

}
