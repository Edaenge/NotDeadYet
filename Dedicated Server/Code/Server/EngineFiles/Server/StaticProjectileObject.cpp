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

StaticProjectileObject::StaticProjectileObject(const DynamicProjectileObject* other, bool genID)
{

	if (genID)
		this->GenerateID();
	else
		this->SetID(other->GetID());

	this->zType = other->GetType();
	this->zScale = other->GetScale();
	this->zRot = other->GetRotation();
	this->zPos = other->GetPosition();
	this->zDamage = other->GetDamage();
	this->zWeight = other->GetWeight();
	this->zStacks = other->GetStackSize();
	this->zIconPath = other->GetIconPath();
	this->zVelocity = other->GetVelocity();
	this->zActorModel = other->GetActorModel();
	this->zDescription = other->GetDescription();
	this->zActorObjectName = other->GetActorObjectName();
}

StaticProjectileObject::StaticProjectileObject( const StaticProjectileObject* other, bool genID /*= false*/ )
{
	if (genID)
		this->GenerateID();
	else
		this->SetID(other->GetID());

	this->zRot = other->zRot;
	this->zPos = other->zPos;
	this->zType = other->zType;
	this->zScale = other->zScale;
	this->zStacks = other->zStacks;
	this->zDamage = other->zDamage;
	this->zWeight = other->zWeight;
	this->zIconPath = other->zIconPath;
	this->zVelocity = other->zVelocity;
	this->zActorModel = other->zActorModel;
	this->zDescription = other->zDescription;
	this->zActorObjectName = other->zActorObjectName;
}

StaticProjectileObject::~StaticProjectileObject()
{

}