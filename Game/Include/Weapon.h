/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_WEAPON_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_BOW,
	ITEM_SUB_TYPE_MACHETE,
	ITEM_SUB_TYPE_POCKET_KNIFE
};

 /*! Abstract class for Weapons*/
class Weapon : public Item
{
public:
	Weapon() {this->zDamage = 0; this->zRange = 0;}

	Weapon(const unsigned int itemType, const unsigned int itemSubType, 
		const float damage, const float range)
		: Item(itemType, itemSubType)
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

	virtual std::string ToMessageString( NetworkMessageConverter* NMC )
	{
		std::string msg = Item::ToMessageString(NMC);

		msg += NMC->Convert(MESSAGE_TYPE_WEAPON_DAMAGE, (float)this->zDamage);
		msg += NMC->Convert(MESSAGE_TYPE_WEAPON_RANGE, (float)this->zRange);

		return msg;
	}
	virtual bool Use() = 0;
	virtual void UseWeapon(float& range, float& damage) = 0;
protected:
	float zDamage;
	float zRange;
};