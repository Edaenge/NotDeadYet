/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/
#pragma once

#include "GameFiles/ClientSide/OrganicObject.h"

/*! Class to Handle Animals*/
class AnimalObject : public OrganicObject
{
public:
	AnimalObject(const unsigned int id) : OrganicObject(id){};
	virtual ~AnimalObject(){}
	/*! Updates the Animal*/
	virtual void Update(float deltaTime);
};