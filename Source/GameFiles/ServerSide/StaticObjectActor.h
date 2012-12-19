/*
Made by Edänge Simon Datum(14/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/Actor.h"
#include <vector>

/*Class for Static objects, such as Food, Weapons basically things that cannot move.*/
class StaticObjectActor : public Actor
{
public:
	StaticObjectActor(bool genID = true);
	StaticObjectActor(const std::string& meshModel, bool genID = true);
	virtual ~StaticObjectActor();
	
	virtual void Update(float deltaTime){};

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}

	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}

protected:
	int zWeight;
	int zType;

private:
};