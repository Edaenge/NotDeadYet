/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "StaticObjectActor.h"

class FoodObject : public StaticObjectActor
{
public:
	FoodObject(const bool genID = false);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID if bool is true.*/
	FoodObject(const FoodObject& other, const bool genID = false);
	FoodObject::FoodObject( const FoodObject* other, const bool genID = false);
	/*! */
	virtual ~FoodObject();

	
	float GetHunger() const {return this->zHunger;}

	void SetHunger(const float hunger){this->zHunger = hunger;}
private:
	float zHunger;
};
