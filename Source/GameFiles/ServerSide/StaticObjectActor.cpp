#include "StaticObjectActor.h"


StaticObjectActor::StaticObjectActor(bool genID)
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
}

StaticObjectActor::StaticObjectActor(const std::string& meshModel, bool genID)
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->SetActorModel(meshModel);
}

StaticObjectActor::~StaticObjectActor()
{
}