/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

 /*! Abstract class for Weapons*/
class Weapon : public Item
{
public:
	Weapon() : Item() {this->zDamage = 0; this->zRange = 0;}
	Weapon(const long id, const unsigned int itemType, 
		const float damage, const float range)
		: Item(id, itemType)
	{
		this->zDamage = damage; 
		this->zRange = range;
	}
	virtual ~Weapon(){}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}
	/*! Returns the Weapon Range*/
	float GetRange() const {return this->zRange;}

	void SetDamage(const float damage) {this->zDamage = damage;}
	void SetRange(const float range) {this->zRange = range;}

	virtual bool Use() = 0;
	virtual std::string ToMessageString(NetworkMessageConverter* NMC) = 0;
	virtual void UseWeapon(float& range, float& damage) = 0;
protected:
	float zDamage;
	float zRange;
};