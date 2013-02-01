#include "FoodObject.h"


FoodObject::FoodObject( const bool genID /*= false*/ ) : StaticObjectActor(genID)
{
	this->zHunger = 1.0f;
	this->zWeight = 1;
	this->zActorType = ACTOR_TYPE_STATIC_OBJECT_FOOD;
}


FoodObject::FoodObject( const FoodObject& other, const bool genID /*genID = false*/ )
{

	if(genID)
		GenerateID();
	else
		this->zID = other.GetID();

	
	this->zRot = other.zRot;
	this->zPos = other.zPos;
	this->zType = other.zType;
	this->zScale = other.zScale;
	this->zHunger = other.zHunger;
	this->zStacks = other.zStacks;
	this->zWeight = other.zWeight;
	this->zIconPath = other.zIconPath;
	this->zActorType = other.zActorType;
	this->zActorModel = other.zActorModel;
	this->zDescription = other.zDescription;
	this->zActorObjectName = other.zActorObjectName;
}

FoodObject::FoodObject( const FoodObject* other, const bool genID /*genID = false*/ )
{
	if(genID)
		this->zID = this->GenerateID();
	else
		this->zID = other->GetID();

	this->zRot = other->zRot;
	this->zPos = other->zPos;
	this->zType = other->zType;
	this->zScale = other->zScale;
	this->zWeight = other->zWeight;
	this->zHunger = other->zHunger;
	this->zStacks = other->zStacks;
	this->zIconPath = other->zIconPath;
	this->zActorType = other->zActorType;
	this->zActorModel = other->zActorModel;
	this->zDescription = other->zDescription;
	this->zActorObjectName = other->zActorObjectName;
}

FoodObject::~FoodObject()
{

}