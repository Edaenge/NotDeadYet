/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "GameFiles/AnimationStates.h"
#include "GameFiles/Items/Equipment.h"
#include "GameFiles/ClientSide/OrganicObject.h"

//Keeps track of player Info
class PlayerObject : public OrganicObject
{
public:
	PlayerObject(const unsigned int id) : OrganicObject(id) {this->zEquipment = new Equipment();}
	virtual ~PlayerObject(){if(this->zEquipment) delete this->zEquipment; this->zEquipment = 0;}
	
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
	void SetEquipment(Equipment* eq) {this->zEquipment = eq;}
private:
	float zHunger;
	float zStamina;
	float zHydration;
	Equipment* zEquipment;
};