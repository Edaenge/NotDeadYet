/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
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

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}

	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}

private:
	int zWeight;
	int zType;
};