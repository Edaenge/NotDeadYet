/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/ClientSide/OrganicObject.h"
#include "GameFiles/Items/Equipment.h"

//Keeps track of player Info
class PlayerObject : public OrganicObject
{
public:
	PlayerObject(const unsigned int id) : OrganicObject(id) {this->zEquipment = 0;}
	virtual ~PlayerObject(){}
	
	inline float GetHunger() const {return this->zHunger;}
	inline float GetStamina() const {return this->zStamina;}
	inline float GetHydration() const {return this->zHydration;}

	void SetStamina(const float stamina) {this->zStamina = stamina;}
	void SetHunger(const float hunger) {this->zHunger = hunger;}
	void SetHydration(const float hydration) {this->zHydration = hydration;}

	/*! Updates the Player*/
	virtual void Update(float deltaTime);
	/*! Returns a Pointer to the players Equipment*/
	Equipment* GetEquipmentPtr() const {return this->zEquipment;};
private:
	float zHunger;
	float zStamina;
	float zHydration;
	Equipment* zEquipment;
};