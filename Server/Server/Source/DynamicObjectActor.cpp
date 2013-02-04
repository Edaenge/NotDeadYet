#include "DynamicObjectActor.h"

DynamicObjectActor::DynamicObjectActor(bool genID) : PhysicsActor()
{
	if(genID)
		this->zID = this->GenerateID();

	this->zObjPlayerOwner = -1;
	InitValues();
}

DynamicObjectActor::DynamicObjectActor(const std::string& meshModel, bool genID) : PhysicsActor()
{
	if(genID)
		this->zID = this->GenerateID();

	this->SetActorModel(meshModel);
	this->zObjPlayerOwner = -1;
	this->InitValues();
}

DynamicObjectActor::DynamicObjectActor( const std::string& meshModel, int objOwner, bool genID /*= false*/ ) : PhysicsActor()
{
	if(genID)
		this->zID = this->GenerateID();

	this->SetActorModel(meshModel);
	this->zObjPlayerOwner = objOwner;
	this->InitValues();
}

void DynamicObjectActor::InitValues()
{
	this->zType = 0;
	this->zWeight = 1;
	this->zStackSize = 1;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zPhysicObj = NULL;
	this->zMoving = true;
}

DynamicObjectActor::~DynamicObjectActor()
{
}