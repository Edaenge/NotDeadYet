/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "Item.h"

class Material : public Item
{
public:
	Material(const unsigned int id, const unsigned int itemType);
	virtual ~Material();

	int GetCraftingType() const {return this->zCraftingType;}
	int GetRequiredStackToCraft() const {return this->zRequiredStackToCraft;}

	void SetCraftingType(const int TYPE) {this->zCraftingType = TYPE;}
	void SetRequiredStackToCraft(const int size) {this->zRequiredStackToCraft = size;}

	virtual bool Use() = 0;
	virtual bool IsUsable() = 0;
protected:
	unsigned int zCraftingType;
	unsigned int zRequiredStackToCraft;
};