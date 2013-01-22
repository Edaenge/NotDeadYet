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

DynamicProjectileObject::~DynamicProjectileObject()
{

}

void DynamicProjectileObject::Update(float deltaTime)
{
	Vector3 oldPosition = this->zPos;

	this->zPos += (this->zDirection * deltaTime * this->zVelocity);
	this->zPhysicObj->SetPosition(this->zPos);

	//this->zPos.y += (-9.82f) * deltaTime;

	if (oldPosition == this->zPos)
	{
		this->zMoving = false;
	}
	else
	{
		this->zMoving = true;
	}
}

