#include "FoodObject.h"


FoodObject::FoodObject() : StaticObjectActor()
{
	this->zHunger = 1.0f;
	this->zWeight = 1;
}

FoodObject::FoodObject( const FoodData* data ) : StaticObjectActor()
{
	if(!data)
	{
		this->zHunger = 1.0f;
		this->zWeight = 1;
	}
	else
	{
		this->zHunger = data->zHunger;
		this->zWeight = data->zWeight;
		this->zActorModel = data->zModel;
		this->zType = data->zType;
		this->zActorObjectName = data->zObjName;
	}
}

FoodObject::~FoodObject()
{

}
