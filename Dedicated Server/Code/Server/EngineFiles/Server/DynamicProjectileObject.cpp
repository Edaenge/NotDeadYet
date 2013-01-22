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

	SetRotation(other.GetRotation());
	SetPosition(other.GetPosition());

	this->zType = other.zType;
	SetScale(other.GetScale());
	this->zStacks = other.zStacks;
	this->zDamage = other.zDamage;
	this->zWeight = other.zWeight;
	this->zIconPath = other.zIconPath;
	this->zVelocity = other.zVelocity;
	this->zActorModel = other.zActorModel;
	this->zDescription = other.zDescription;
	this->zActorObjectName = other.zActorObjectName;
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
	SetScale(other->GetScale());
	SetScale(other->GetScale());
	SetPosition(other->GetPosition());
}

DynamicProjectileObject::~DynamicProjectileObject()
{

}

void DynamicProjectileObject::Update(float deltaTime)
{
	Vector3 oldPosition = GetPosition();
	Vector3 modifiedPos = oldPosition;

	modifiedPos += (this->zDirection * deltaTime * this->zVelocity);
	SetPosition(modifiedPos);

	//this->zPos.y += (-9.82f) * deltaTime;

	if (oldPosition == modifiedPos)
	{
		this->zMoving = false;
	}
	else
	{
		this->zMoving = true;
	}
}

