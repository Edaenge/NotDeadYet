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

	int GetCraftingType() const {return this->zCraftingType;}
	void SetCraftingType(const int TYPE) {this->zCraftingType = TYPE;}

	virtual bool Use() = 0;
protected:
	int zCraftingType;
};