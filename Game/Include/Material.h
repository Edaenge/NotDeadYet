/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

static const enum M_MATERIAL_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_SMALL_STICK,
	ITEM_SUB_TYPE_MEDIUM_STICK,
	ITEM_SUB_TYPE_LARGE_STICK,
	ITEM_SUB_TYPE_THREAD
};

class Material : public Item
{
public:
	Material(const long id, const unsigned int itemType, const unsigned int craftingType, const int stacksRequiredToCraft);
	Material(const long id, const unsigned int itemType, const unsigned int itemSubType, const unsigned int craftingType, const int stacksRequiredToCraft);
	Material(const Material& other);
	Material(const Material* other);
	virtual ~Material();

	int GetCraftingType() const {return this->zCraftingType;}
	int GetRequiredStackToCraft() const {return this->zRequiredStackToCraft;}

	void SetCraftingType(const int TYPE) {this->zCraftingType = TYPE;}
	void SetRequiredStackToCraft(const int size) {this->zRequiredStackToCraft = size;}

	virtual bool Use();
	virtual bool IsUsable();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);

private:
	int zCraftingType;
	int zRequiredStackToCraft;
};