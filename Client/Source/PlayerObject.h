/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "OrganicObject.h"

//Keeps track of player Info
class PlayerObject : public OrganicObject
{
public:
	PlayerObject(const long id);
	virtual ~PlayerObject(){}
	
	inline float GetHunger() const {return this->zHunger;}
	inline float GetHydration() const {return this->zHydration;}

	void SetHunger(const float hunger) {this->zHunger = hunger;}
	void SetHydration(const float hydration) {this->zHydration = hydration;}
	/*! Updates the Player*/
	virtual void Update(float deltaTime);

private:
	float zHunger;
	float zHydration;
};