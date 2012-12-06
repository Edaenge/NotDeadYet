/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/ClientSide/WorldObject.h"
#include "GameFiles/AnimationStates.h"

class Animal : public WorldObject
{
public:
	Animal() : WorldObject(){this->zAnimalState = IDLE;}
	virtual ~Animal(){}
	/*! Sets the Player State*/
	inline void SetAnimalState(const int state) {this->zAnimalState = state;}
	/*! Returns Current Player State*/
	inline int GetAnimalState() const {return this->zAnimalState;}
	/*! Updates the Animal data*/
	void Update(float dt){};
private:
	int zAnimalState;
};