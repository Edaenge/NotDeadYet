/*
	Made by Jensen Christopher Datum(03/03/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_MISC_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_REGULAR_TRAP,
	ITEM_SUB_TYPE_CAMPFIRE
};

class Misc : public Item
{
public:
	Misc();
	Misc(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType);
	Misc(const unsigned int itemType, const unsigned int itemSubType);
	Misc(const Misc& other);
	virtual ~Misc();

	virtual bool Use();
private:
};