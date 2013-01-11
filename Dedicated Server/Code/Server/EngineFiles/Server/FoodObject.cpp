#include "FoodObject.h"


FoodObject::FoodObject( const bool genID /*= false*/ ) : StaticObjectActor(genID)
{
	this->zHunger = 1.0f;
	this->zWeight = 1;
}


FoodObject::FoodObject( const FoodObject& other, const bool genID /*genID = false*/ )
{

	if(genID)
		GenerateID();
	else
		this->SetID(other.GetID());

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
	this->zStacks = other.zStacks;
}

FoodObject::FoodObject( const FoodObject* other, const bool genID /*genID = false*/ )
{
	if(genID)
		GenerateID();
	else
		this->SetID(other->GetID());

	this->zHunger = other->zHunger;
	this->zWeight = other->zWeight;
	this->zActorModel = other->zActorModel;
	this->zType = other->zType;
	this->zActorObjectName = other->zActorObjectName;
	this->zIconPath = other->zIconPath;
	this->zDescription = other->zDescription;
	this->zScale = other->zScale;
	this->zRot = other->zRot;
	this->zPos = other->zPos;
	this->zStacks = other->zStacks;
}

FoodObject::~FoodObject()
{

}