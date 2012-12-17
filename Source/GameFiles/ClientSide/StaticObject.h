/*
	Made by Jensen Christopher Datum(06/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "GameFiles/ClientSide/WorldObject.h"
/*! Class to Handle Static Objects*/
class StaticObject : public WorldObject
{
public:
	StaticObject(const unsigned int id) : WorldObject(id){}
	virtual ~StaticObject(){}

	virtual void Update(float dt){};
private:

};