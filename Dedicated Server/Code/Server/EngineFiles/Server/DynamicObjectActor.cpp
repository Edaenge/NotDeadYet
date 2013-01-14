#include "DynamicObjectActor.h"

DynamicObjectActor::DynamicObjectActor(bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zObjPlayerOwner = -1;
	InitValues();
}

DynamicObjectActor::DynamicObjectActor(const std::string& meshModel, bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->SetActorModel(meshModel);
	this->zObjPlayerOwner = -1;
	this->InitValues();
}

DynamicObjectActor::DynamicObjectActor( const std::string& meshModel, int objOwner, bool genID /*= false*/ )
{
	if(genID)
		this->GenerateID();

	this->SetActorModel(meshModel);
	this->zObjPlayerOwner = objOwner;
	this->InitValues();
}

void DynamicObjectActor::InitValues()
{
	this->zType = 0;
	this->zWeight = 1;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zDirection = Vector3(1, 0, 0);
	this->zPhysicObj = NULL;
}

DynamicObjectActor::~DynamicObjectActor()
{
}