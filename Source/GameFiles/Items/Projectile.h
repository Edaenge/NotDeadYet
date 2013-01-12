/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

class Projectile : public Item
{
public:
	Projectile(const unsigned int id, const unsigned int weight, const std::string& name, 
		const unsigned int itemType, const std::string& description, float velocity, float damage);
	virtual ~Projectile();

	void SetVelocity(float vel) {this->zVelocity = vel;}
	void SetDamage(float damage) {this->zDamage = damage;}

	float GetVelocity() const {return this->zVelocity;}
	float GetDamage() const {return this->zDamage;}

	virtual bool Use();
private:
	float zDamage;
	float zVelocity;
};