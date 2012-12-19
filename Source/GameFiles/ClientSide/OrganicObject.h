/*
	Made by Jensen Christopher Datum(19/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/ClientSide/MovingObject.h"

class OrganicObject : public MovingObject
{
public:
	OrganicObject(const unsigned int id) : MovingObject(id) {}
	virtual ~OrganicObject() {}
	/*! Return the Health*/
	inline float GetHealth() const {return this->zHealth;}
	/*! Sets the Health*/
	inline void SetHealth(const float health) {this->zHealth = health;}
protected:
	float zHealth;
private:
};