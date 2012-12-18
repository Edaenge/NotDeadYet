/*
Made by Edänge Simon Datum(17/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/StaticObjectActor.h"
#include "GameFiles/ServerSide/ObjectManager.h"

class FoodObject : public StaticObjectActor
{
public:
	FoodObject();
	/*! Use ObjectManager to Get Data*/
	FoodObject(const FoodData* data);
	virtual ~FoodObject();


	float GetHunger() const {return this->zHunger;}
	void SetHealth(const float hunger){this->zHunger = hunger;}

private:
	float zHunger;

};
