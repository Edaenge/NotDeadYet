#include "StaticProjectileObject.h"


StaticProjectileObject::StaticProjectileObject( const bool genID /*= true*/ ) : StaticObjectActor(genID)
{
	this->zSpeed = 1.0f;
	this->zStacks = 1;
	this->zWeight = 1;
}


StaticProjectileObject::StaticProjectileObject(const StaticProjectileObject& other, bool genID)
{
	if (genID)
		this->GenerateID();
	else
		this->SetID(other.GetID());

	this->zRot = other.zRot;
	this->zPos = other.zPos;

	this->zType = other.zType;
	this->zScale = other.zScale;
	this->zStacks = other.zStacks;
	this->zDamage = other.zDamage;
	this->zWeight = other.zWeight;
	this->zIconPath = other.zIconPath;
	this->zSpeed = other.zSpeed;
	this->zActorModel = other.zActorModel;
	this->zDescription = other.zDescription;
	this->zActorObjectName = other.zActorObjectName;
	this->zModelLength = other.zModelLength;
	
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
	this->zSpeed = other->GetSpeed();
	this->zActorModel = other->GetActorModel();
	this->zDescription = other->GetDescription();
	this->zActorObjectName = other->GetActorObjectName();
	this->zModelLength = other->GetModelLength();
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
	this->zSpeed = other->zSpeed;
	this->zActorModel = other->zActorModel;
	this->zDescription = other->zDescription;
	this->zActorObjectName = other->zActorObjectName;
	this->zModelLength = other->zModelLength;
}

StaticProjectileObject::~StaticProjectileObject()
{

}