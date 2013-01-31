#include "ContainerObject.h"


ContainerObject::ContainerObject(const bool genID) : StaticObjectActor(genID)
{
	this->zMaxUse = 0;
	this->zUsesLeft = 0;
	this->zActorType = ACTOR_TYPE_STATIC_OBJECT_CONTAINER;
}


ContainerObject::ContainerObject(const ContainerObject& other, const bool genID /*= false*/)
{
	if(genID)
		this->GenerateID();
	else
		this->SetID(other.GetID());

	this->zRot = other.zRot;
	this->zPos = other.zPos;
	this->zType = other.zType;
	this->zScale = other.zScale;
	this->zWeight = other.zWeight;
	this->zMaxUse = other.zMaxUse;
	this->zStacks = other.zStacks;
	this->zUsesLeft = other.zUsesLeft;
	this->zIconPath = other.zIconPath;
	this->zActorModel = other.zActorModel;
	this->zDescription = other.zDescription;
	this->zActorObjectName = other.zActorObjectName;
}

ContainerObject::ContainerObject(const ContainerObject* other, const bool genID /*= false*/)
{
	if(genID)
		this->GenerateID();
	else
		this->SetID(other->GetID());

	this->zRot = other->zRot;
	this->zPos = other->zPos;
	this->zType = other->zType;
	this->zScale = other->zScale;
	this->zWeight = other->zWeight;
	this->zMaxUse = other->zMaxUse;
	this->zStacks = other->zStacks;
	this->zUsesLeft = other->zUsesLeft;
	this->zIconPath = other->zIconPath;
	this->zActorModel = other->zActorModel;
	this->zDescription = other->zDescription;
	this->zActorObjectName = other->zActorObjectName;
}


ContainerObject::~ContainerObject()
{

}
