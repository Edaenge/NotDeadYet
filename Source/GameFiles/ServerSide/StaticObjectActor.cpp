#include "StaticObjectActor.h"


StaticObjectActor::StaticObjectActor( bool genID /*= true*/ )
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
}

StaticObjectActor::StaticObjectActor( const std::string& meshModel, bool genID /*= true*/ )
{
	if(genID)
		this->GenerateID();

	this->zType = 0;
}

StaticObjectActor::~StaticObjectActor()
{
}