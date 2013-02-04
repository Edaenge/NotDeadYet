/*
	Made by Jensen Christopher Datum(19/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "MovingObject.h"

class OrganicObject : public MovingObject
{
public:
	OrganicObject(const long id) : MovingObject(id) {this->zHealth = 0.0f;}
	virtual ~OrganicObject() {}
	inline float GetStamina() const {return this->zStamina;}
	/*! Return the Health*/
	inline float GetHealth() const {return this->zHealth;}

	void SetStamina(const float stamina) {this->zStamina = stamina;}
	/*! Sets the Health*/
	inline void SetHealth(const float health) {this->zHealth = health;}
protected:
	float zHealth;
	float zStamina;
private:
};