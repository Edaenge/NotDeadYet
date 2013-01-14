#include "ContainerObject.h"


ContainerObject::ContainerObject(const bool genID) : StaticObjectActor(genID)
{
	this->zMaxUse = 0;
	this->zUsesLeft = 0;
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
	this->zActorObjectName = other.zActorObjectName;
	this->zDescription = other.zDescription;
	
}

ContainerObject::ContainerObject( const ContainerObject* other, const bool genID /*= false*/ )
{
	if(genID)
		this->GenerateID();
	else
		this->SetID(other->GetID());

	this->zMaxUse = other->zMaxUse;
	this->zUsesLeft = other->zUsesLeft;
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


ContainerObject::~ContainerObject()
{

}
