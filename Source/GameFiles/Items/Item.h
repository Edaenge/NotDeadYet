/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include <string>
static const enum M_ITEM_TYPE
{
	ITEM_TYPE_WEAPON_RANGED_BOW,
	ITEM_TYPE_WEAPON_RANGED_ROCK,
	ITEM_TYPE_WEAPON_MELEE_AXE,
	ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE,
	ITEM_TYPE_GEAR_HEAD,
	ITEM_TYPE_GEAR_CHEST,
	ITEM_TYPE_GEAR_LEGS,
	ITEM_TYPE_GEAR_BOOTS,
	ITEM_TYPE_FOOD_MEAT
};
/*! Abstract class for Items*/
class Item
{
public:
	Item(const int id, const unsigned int itemType);
	Item(const unsigned int id, const unsigned int weight, const std::string& name, const unsigned int itemType);
	virtual ~Item(){}
	/*! Returns Item ID*/
	inline int GetID() const {return this->zID;}
	/*! Returns the Item Weight*/
	inline unsigned int GetWeight() const {return this->zWeight;}
	/*! Sets the Item ID*/
	void SetID(const unsigned int id) {this->zID = id;}
	/*! Sets the Item Weight*/
	void SetWeight(const unsigned int weight) {this->zWeight = weight;}

	virtual void Use() = 0;

private:
	void InitValues(const unsigned int id = 0, const unsigned int itemType = 0, const unsigned int weight = 0, const std::string& name = "Unknown");
private:
	unsigned int zID;
	unsigned int zWeight;
	std::string zItemName;
	unsigned int zItemType;
};