/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

class Material : public Item
{
public:
	Material(const unsigned int id, const unsigned int itemType, float velocity, float damage);
	virtual ~Material();

	void SetVelocity(float vel) {this->zVelocity = vel;}
	void SetDamage(float damage) {this->zDamage = damage;}

	float GetVelocity() const {return this->zVelocity;}
	float GetDamage() const {return this->zDamage;}

	virtual bool Use();
	virtual Item* Craft();
private:
	float zDamage;
	float zVelocity;
};