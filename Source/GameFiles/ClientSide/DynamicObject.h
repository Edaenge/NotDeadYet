/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/MovingObject.h"

/*! Class to Handle Dynamic Objects*/
class DynamicObject : public MovingObject
{
public:
	DynamicObject(const unsigned int id);
	virtual ~DynamicObject(){}
	/*! Updates the Object*/
	virtual void Update(float deltaTime);
};