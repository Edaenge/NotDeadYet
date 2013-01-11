#include "ProjectileObject.h"


ProjectileObject::ProjectileObject( const bool genID /*= true*/ ) : DynamicObjectActor(genID)
{
	this->zVelocity = 1.0f;
	this->zStacks = 1;
	this->zWeight = 1;
}


ProjectileObject::ProjectileObject( const ProjectileObject& other )
{
	this->GenerateID();
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

ProjectileObject& ProjectileObject::operator= (const ProjectileObject& other)
{
	if(this == &other)
		return *this;

	this->GenerateID();
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
	
	return *this;
}

ProjectileObject::~ProjectileObject()
{

}

void ProjectileObject::Update(float deltaTime)
{
	this->zPos += (this->zDirection * deltaTime * this->zVelocity);

	//this->zPos.y += (-9.82f) * deltaTime;
}