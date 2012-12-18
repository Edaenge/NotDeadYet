#include "StaticObjectActor.h"


StaticObjectActor::StaticObjectActor()
{
	this->zMeshModel = "none";
	this->zType = 0;
}

StaticObjectActor::StaticObjectActor( const std::string& meshModel )
{
	this->zMeshModel = meshModel;
}

StaticObjectActor::~StaticObjectActor()
{
}