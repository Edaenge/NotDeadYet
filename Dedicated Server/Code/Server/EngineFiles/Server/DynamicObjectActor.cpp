#include "DynamicObjectActor.h"


DynamicObjectActor::DynamicObjectActor(bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zDirection = Vector3(1, 0, 0);
}

DynamicObjectActor::DynamicObjectActor(const std::string& meshModel, bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->SetActorModel(meshModel);
	this->zDescription = "none";
	this->zIconPath = "none";
}

DynamicObjectActor::~DynamicObjectActor()
{
}