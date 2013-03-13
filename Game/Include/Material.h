/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Item.h"

//NOTE: Do not rearrange!!!.
static const enum M_MATERIAL_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_SMALL_STICK,
	ITEM_SUB_TYPE_MEDIUM_STICK,
	ITEM_SUB_TYPE_LARGE_STICK,
	ITEM_SUB_TYPE_THREAD,
	ITEM_SUB_TYPE_DISENFECTANT_LEAF,
	MATERIALS_COUNT
};

class Material : public Item
{
public:
	Material();
	Material(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType);	
	Material(const unsigned int itemType, const unsigned int itemSubType);	
	Material(const Material& other);
	virtual ~Material();

	virtual bool Use();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);
};