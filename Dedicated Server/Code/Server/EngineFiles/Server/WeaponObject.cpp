#include "WeaponObject.h"



WeaponObject::WeaponObject(const bool genID /* = true */) : StaticObjectActor(genID)
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

	this->zPos = other.zPos;
	this->zDamage = other.zDamage;
	this->zRange = other.zRange;
	this->zWeight = other.zWeight;
	this->zActorModel = other.zActorModel;
	this->zType = other.zType;
	this->zActorObjectName = other.zActorObjectName;
	this->zDescription = other.zDescription;
	this->zIconPath = other.zIconPath;
	this->zScale = other.zScale;
	this->zRot = other.zRot;

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
