/*
	Made by Jensen Christopher Datum(02/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/ServerSide/StaticObjectActor.h"

class ContainerObject : public StaticObjectActor
{
public:
	ContainerObject(const bool genID = true);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	ContainerObject(const ContainerObject& other);
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	ContainerObject& operator= (const ContainerObject& other);
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
