/*
	Made by Jensen Christopher Datum(02/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "StaticObjectActor.h"

static const enum M_CONTAINER_OBJECT_SUB_TYPE
{
	OBJECT_SUB_TYPE_CANTEEN,
	OBJECT_SUB_TYPE_WATER_BOTTLE
};

class ContainerObject : public StaticObjectActor
{
public:
	ContainerObject(const bool genID = false);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	ContainerObject(const ContainerObject& other, const bool genID = false);
	ContainerObject(const ContainerObject* other, const bool genID = false);

	/*! */
	virtual ~ContainerObject();

	int GetMaxUses() const {return this->zMaxUse;}
	int GetCurrentUses() const {return this->zUsesLeft;}

	void SetMaxUses(const int max){this->zMaxUse = max;}
	void SetCurrentUses(const int usesLeft){this->zUsesLeft = usesLeft;}

private:
	int zMaxUse;
	int zUsesLeft;

};
