#include "WeaponObject.h"



WeaponObject::WeaponObject(const bool genID /* = false */) : StaticObjectActor(genID)
{
	this->zWeight = 1;
	this->zRange = 1.0f;
	this->zDamage = 0.0f;
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

	this->zPos = other->zPos;
	this->zDamage = other->zDamage;
	this->zRange = other->zRange;
	this->zWeight = other->zWeight;
	this->zActorModel = other->zActorModel;
	this->zType = other->zType;
	this->zActorObjectName = other->zActorObjectName;
	this->zDescription = other->zDescription;
	this->zIconPath = other->zIconPath;
	this->zScale = other->zScale;
	this->zRot = other->zRot;
}


WeaponObject::~WeaponObject()
{

}
