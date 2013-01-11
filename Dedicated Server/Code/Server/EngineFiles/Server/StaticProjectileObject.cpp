#include "StaticProjectileObject.h"


StaticProjectileObject::StaticProjectileObject( const bool genID /*= true*/ ) : StaticObjectActor(genID)
{
	this->zVelocity = 1.0f;
	this->zStacks = 1;
	this->zWeight = 1;
}


StaticProjectileObject::StaticProjectileObject(const StaticProjectileObject& other, bool genID)
{
	if (genID)
		this->GenerateID();
	else
		this->SetID(other.GetID());

	this->zVelocity = other.zVelocity;
	this->zStacks = other.zStacks;
	this->zDamage = other.zDamage;
	this->zWeight = other.zWeight;
	this->zActorModel = other.zActorModel;
	this->zType = other.zType;
	this->zActorObjectName = other.zActorObjectName;
	this->zIconPath = other.zIconPath;
	this->zDescription = other.zDescription;
	this->zScale = other.zScale;
	this->zRot = other.zRot;
	this->zPos = other.zPos;
}

//StaticProjectileObject::StaticProjectileObject(const DynamicProjectileObject* other, bool genID)
//{
//	this->SetID(other->GetID());
//	this->zVelocity = other->GetVelocity();
//	this->zStacks = other->GetStackSize();
//	this->zDamage = other->GetDamage();
//	this->zWeight = other->GetWeight();
//	this->zActorModel = other->GetActorModel();
//	this->zType = other->GetType();
//	this->zActorObjectName = other->GetActorObjectName();
//	this->zIconPath = other->GetIconPath();
//	this->zDescription = other->GetDescription();
//	this->zScale = other->GetScale();
//	this->zRot = other->GetRotation();
//	this->zPos = other->GetPosition();
//}

StaticProjectileObject::~StaticProjectileObject()
{

}