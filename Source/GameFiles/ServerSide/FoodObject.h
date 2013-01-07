/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/StaticObjectActor.h"

class FoodObject : public StaticObjectActor
{
public:
	FoodObject(const bool genID = true);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	FoodObject(const FoodObject& other);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	FoodObject& operator= (const FoodObject& other);
	/*! */
	virtual ~FoodObject();


	float GetHunger() const {return this->zHunger;}
	void SetHunger(const float hunger){this->zHunger = hunger;}

private:
	float zHunger;

};
