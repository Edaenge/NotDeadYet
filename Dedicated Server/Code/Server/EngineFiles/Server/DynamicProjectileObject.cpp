#include "DynamicProjectileObject.h"


DynamicProjectileObject::DynamicProjectileObject(const bool genID /*= true*/) : DynamicObjectActor(genID)
{
	this->zVelocity = 5.0f;
	this->zStacks = 1;
	this->zWeight = 1;
	this->zMoving = true;
}


DynamicProjectileObject::DynamicProjectileObject(const DynamicProjectileObject& other, bool genID)
{
	if (genID)
		this->GenerateID();
	else
		this->SetID(other.GetID());
	
	this->zVelocity = other.zVelocity;
	this->zStacks = other.zStacks;
	this->zDamage = other.zDamage;
	this->zDirection = other.zDirection;
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
DynamicProjectileObject::DynamicProjectileObject(const DynamicProjectileObject* other, bool genID)
{
	if (genID)
		this->GenerateID();
	else
		this->SetID(other->GetID());

	this->zVelocity = other->zVelocity;
	this->zStacks = other->zStacks;
	this->zDamage = other->zDamage;
	this->zDirection = other->zDirection;
	this->zWeight = other->zWeight;
	this->zActorModel = other->zActorModel;
	this->zType = other->zType;
	this->zActorObjectName = other->zActorObjectName;
	this->zIconPath = other->zIconPath;
	this->zDescription = other->zDescription;
	this->zScale = other->zScale;
	this->zRot = other->zRot;
	this->zPos = other->zPos;
}

//DynamicProjectileObject::DynamicProjectileObject(const StaticProjectileObject* other, bool genID)
//{
//	if (genID)
//		this->GenerateID();
//	else
//		this->SetID(other->GetID());
//
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
//
//	this->zMoving = true;
//}

DynamicProjectileObject::~DynamicProjectileObject()
{

}

void DynamicProjectileObject::Update(float deltaTime)
{
	Vector3 oldPosition = this->zPos;

	this->zPos += (this->zDirection * deltaTime * this->zVelocity);

	//this->zPos.y += (-9.82f) * deltaTime;

	if (oldPosition.x == this->zPos.x && oldPosition.y == this->zPos.x && oldPosition.z == this->zPos.z)
	{
		this->zMoving = false;
	}
	else
	{
		this->zMoving = true;
	}
}