/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/ClientSide/WorldObject.h"
/*! Class to Handle Static Objects*/
class StaticObject : public WorldObject
{
public:
	StaticObject(const unsigned int id) : WorldObject(id){}
	virtual ~StaticObject(){}
	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}

	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}

private:
	int zWeight;
	int zType;
};