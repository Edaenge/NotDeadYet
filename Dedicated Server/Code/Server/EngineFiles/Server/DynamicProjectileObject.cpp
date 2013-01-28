#include "DynamicProjectileObject.h"


DynamicProjectileObject::DynamicProjectileObject(const bool genID /*= true*/) : DynamicObjectActor(genID)
{
	this->zSpeed = 5.0f;
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


	this->zPhysicObj = other.zPhysicObj;

	this->zType = other.zType;
	this->zStacks = other.zStacks;
	this->zDamage = other.zDamage;
	this->zWeight = other.zWeight;
	this->zIconPath = other.zIconPath;
	this->zSpeed = other.zSpeed;
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

	this->zSpeed = other->zSpeed;
	this->zStacks = other->zStacks;
	this->zDamage = other->zDamage;
	this->zWeight = other->zWeight;
	this->zActorModel = other->zActorModel;
	this->zType = other->zType;
	this->zActorObjectName = other->zActorObjectName;
	this->zIconPath = other->zIconPath;
	this->zDescription = other->zDescription;
	this->zPhysicObj = other->zPhysicObj;
}

DynamicProjectileObject::~DynamicProjectileObject()
{

}

void DynamicProjectileObject::Update(float deltaTime)
{
	if(!this->zPhysicObj)
		return;

	Vector3 oldPosition = GetPosition();
	
	this->zPhysicObj->Integrate(deltaTime);

	Vector3 ArrowDirection = Vector3(0,0,-1);
	Vector3 CameraDirection = zPhysicObj->GetVelocity();
	ArrowDirection.Normalize();
	CameraDirection.Normalize();

	Vector3 around = ArrowDirection.GetCrossProduct(CameraDirection);
	float angle = acos(ArrowDirection.GetDotProduct(CameraDirection) / (ArrowDirection.GetLength() * CameraDirection.GetLength()));
	zPhysicObj->SetQuaternion(Vector4(0,0,0,1));
	zPhysicObj->RotateAxis(around, angle);

	if (oldPosition == GetPosition())
	{
		this->zMoving = false;
	}
	else
	{
		this->zMoving = true;
	}
}

