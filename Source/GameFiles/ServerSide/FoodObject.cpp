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
		this->zMeshModel = data->zModel;
		this->zType = data->zType;
	}
}

FoodObject::~FoodObject()
{

}
