/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_PROJECTILE_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_ARROW,
	ITEM_SUB_TYPE_ROCK
};

class Projectile : public Item
{
public:
	Projectile(const long id, const unsigned int itemType, float velocity, float damage);
	Projectile(const long id, const unsigned int itemType, const unsigned int itemSubType, float velocity, float damage);
	Projectile(const Projectile& other);
	Projectile(const Projectile* other);
	virtual ~Projectile();

	void SetVelocity(float vel) {this->zVelocity = vel;}
	void SetDamage(float damage) {this->zDamage = damage;}

	float GetVelocity() const {return this->zVelocity;}
	float GetDamage() const {return this->zDamage;}

	virtual bool Use();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);
private:
	float zDamage;
	float zVelocity;
};