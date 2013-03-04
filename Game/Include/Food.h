/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_FOOD_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_DEER_FOOD,
	ITEM_SUB_TYPE_WOLF_FOOD
};

class Food : public Item
{
public:
	Food();
	Food(const unsigned int ID, const unsigned int Type, const unsigned int SubType, const float hunger);
	Food(const unsigned int Type, const unsigned int SubType, const float hunger);
	Food(const Food& other);
	virtual ~Food();
	/*! Decreases Stacks by 1.*/
	virtual bool Use();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);

	float GetHunger() const {return this->zHunger;}

	void SetHunger(const float hunger) {this->zHunger = hunger;}

private:
	float zHunger;
};