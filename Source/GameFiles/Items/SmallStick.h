/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

class SmallStick : public Item
{
public:
	SmallStick(const unsigned int id, const unsigned int itemType);
	virtual ~SmallStick();

	virtual bool Use();
	virtual Item* Craft();
private:
};