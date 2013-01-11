/*
Made by Edänge Simon Datum(17/12/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "StaticObjectActor.h"

class FoodObject : public StaticObjectActor
{
public:
	FoodObject(const bool genID = true);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID if bool is true.*/
	FoodObject(const FoodObject& other, const bool genID = false);
	FoodObject::FoodObject( const FoodObject* other, const bool genID = false);
	/*! */
	virtual ~FoodObject();

	
	float GetHunger() const {return this->zHunger;}
	/*! Returns number of stacks the item has.*/
	unsigned int GetStackSize() const {return this->zStacks;}

	void SetHunger(const float hunger){this->zHunger = hunger;}
	void ModifyStackSize(const unsigned int size) {this->zStacks += size;}
private:
	float zHunger;
	int zStacks;
};
