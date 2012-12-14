/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Item.h"

class Weapon : public Item
{
public:
	Weapon(const unsigned int id): Item(id)
	{
		this->zDamage = 0.0f; 
		this->zRange = 0.0f;
	}
	Weapon(unsigned int id, float damage, float range) : Item(id)
	{
		this->zDamage = damage; 
		this->zRange = range;
	}
	Weapon(const unsigned int id, const unsigned int weight, const std::string& name, 
		const float damage, const float range) : Item(id, weight, name)
	{
		this->zDamage = damage; 
		this->zRange = range;
	}
	virtual ~Weapon(){}
	/*! Returns The Weapon Damage*/
	float GetDamage() const {return this->zDamage;}
	/*! Returns the Weapon Range*/
	float GetRange() const {return this->zRange;}

	virtual void UseWeapon(float& range, float& damage) = 0;
	virtual void Use() = 0;
protected:
	float zDamage;
	float zRange;
};