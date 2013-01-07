#include "StaticObjectActor.h"


StaticObjectActor::StaticObjectActor(bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->zDescription = "none";
	this->zIconPath = "none";
}

StaticObjectActor::StaticObjectActor(const std::string& meshModel, bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->SetActorModel(meshModel);
	this->zDescription = "none";
	this->zIconPath = "none";
}

StaticObjectActor::~StaticObjectActor()
{
}