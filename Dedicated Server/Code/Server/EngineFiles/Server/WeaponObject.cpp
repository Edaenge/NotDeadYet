#include "WeaponObject.h"



WeaponObject::WeaponObject(const bool genID /* = false */) : StaticObjectActor(genID)
{
	this->zWeight = 1;
	this->zRange = 1.0f;
	this->zDamage = 0.0f;
	this->zActorType = ACTOR_TYPE_STATIC_OBJECT_WEAPON;
}

WeaponObject::WeaponObject( const WeaponObject& other, const bool genID /*= false*/)
{

	if(genID)
		GenerateID();
	else
		this->SetID(other.GetID());

	this->zRot = other.zRot;
	this->zPos = other.zPos;
	this->zType = other.zType;
	this->zRange = other.zRange;
	this->zScale = other.zScale;
	this->zDamage = other.zDamage;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zIconPath = other.zIconPath;
	this->zActorModel = other.zActorModel;
	this->zDescription = other.zDescription;
	this->zActorObjectName = other.zActorObjectName;
}

WeaponObject::WeaponObject( const WeaponObject* other, const bool genID /*= false*/ )
{
	if(genID)
		GenerateID();
	else
		this->SetID(other->GetID());

	this->zRot = other->zRot;
	this->zPos = other->zPos;
	this->zType = other->zType;
	this->zRange = other->zRange;
	this->zScale = other->zScale;
	this->zWeight = other->zWeight;
	this->zDamage = other->zDamage;
	this->zStacks = other->zStacks;
	this->zIconPath = other->zIconPath;
	this->zActorModel = other->zActorModel;
	this->zDescription = other->zDescription;
	this->zActorObjectName = other->zActorObjectName;
}


WeaponObject::~WeaponObject()
{

}
