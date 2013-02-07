/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_GEAR_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_HEAD,
	ITEM_SUB_TYPE_CHEST,
	ITEM_SUB_TYPE_LEGS,
	ITEM_SUB_TYPE_BOOTS
};

class Gear : public Item
{
public:
	Gear(const unsigned int ID, const unsigned int Type, const unsigned int SubType);
	Gear(const Gear& other);
	Gear(const Gear* other);
	virtual ~Gear();
	virtual bool Use();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);
private:

};