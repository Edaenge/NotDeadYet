/*
Made by Ed�nge Simon Datum(14/12/12 created) 
for project desperation* at Blekinge tekniska h�gskola.
*/

#pragma once

//static const enum STATIC_OBJECT_TYPE
//{
//	//STATIC_OBJECT_TYPE_
//};

#include "GameFiles/ServerSide/Actor.h"

class StaticObjectActor : public Actor
{
public:
	StaticObjectActor();
	virtual ~StaticObjectActor();
	
	virtual void Update(float deltaTime){};

protected:

private:
};