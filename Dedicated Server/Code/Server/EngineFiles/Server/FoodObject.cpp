#include "FoodObject.h"


FoodObject::FoodObject( const bool genID /*= true*/ ) : StaticObjectActor(genID)
{
	this->zHunger = 1.0f;
	this->zWeight = 1;
}


FoodObject::FoodObject( const FoodObject& other )
{
	this->GenerateID();
	this->zHunger = other.zHunger;
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

FoodObject& FoodObject::operator= (const FoodObject& other)
{
	if(this == &other)
		return *this;

	this->GenerateID();
	this->zHunger = other.zHunger;
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

FoodObject::~FoodObject()
{

}