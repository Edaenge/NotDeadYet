/*
Made by Edänge Simon Datum(14/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/Actor.h"
#include <vector>

class StaticObjectActor : public Actor
{
public:
	StaticObjectActor();
	StaticObjectActor(const std::string& meshModel);
	virtual ~StaticObjectActor();
	
	virtual void Update(float deltaTime){};

	int GetWeight() const {return this->zWeight;}

protected:
	std::string zMeshModel;
	int zWeight;
	int zType;

private:
};