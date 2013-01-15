/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

class Material : public Item
{
public:
	Material(const unsigned int id, const unsigned int itemType);
	virtual ~Material();

	virtual bool Use() = 0;
	virtual Item* Craft() = 0;
private:
};