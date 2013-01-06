#include "GameFiles/ServerSide/ContainerObject.h"


ContainerObject::ContainerObject(const bool genID) : StaticObjectActor(genID)
{
	this->zMaxUse = 0;
	this->zUsesLeft = 0;
}


ContainerObject::ContainerObject(const ContainerObject& other)
{
	this->GenerateID();
	this->zMaxUse = other.zMaxUse;
	this->zUsesLeft = other.zUsesLeft;
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

ContainerObject& ContainerObject::operator= (const ContainerObject& other)
{
	if(this == &other)
		return *this;

	this->GenerateID();
	this->zMaxUse = other.zMaxUse;
	this->zUsesLeft = other.zUsesLeft;
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

ContainerObject::~ContainerObject()
{

}
