#include "StaticObjectActor.h"


StaticObjectActor::StaticObjectActor(bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zStacks = 1;
	this->zScale = Vector3(0.5f,0.5f,0.5f);
	this->zPos = Vector3(0,1,0);
	this->zModelLength = 0;
}

StaticObjectActor::StaticObjectActor(const std::string& meshModel, bool genID) : Actor()
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
	this->zWeight = 1;
	this->zActorModel = meshModel;
	this->zDescription = "none";
	this->zIconPath = "none";
	this->zStacks = 1;
	this->zScale = Vector3(0.5f,0.5f,0.5f);
	this->zPos = Vector3(0,1,0);
	this->zModelLength = 0;
}

StaticObjectActor::~StaticObjectActor()
{
}