/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "StaticObjectActor.h"

static const enum M_MATERIAL_OBJECT_SUB_TYPE
{
	OBJECT_SUB_TYPE_SMALL_STICK,
	OBJECT_SUB_TYPE_MEDIUM_STICK,
	OBJECT_SUB_TYPE_LARGE_STICK,
	OBJECT_SUB_TYPE_THREAD
};

class MaterialObject : public StaticObjectActor
{
public:
	MaterialObject(const bool genID = false);
	virtual ~MaterialObject();
	/*! Note: ID from Actor will not be copied. IT will auto generate a new ID.*/
	MaterialObject(const MaterialObject& other, bool genID = false);
	MaterialObject(const MaterialObject* other, bool genID = false);

	int GetCraftingType() const {return this->zCraftingType;}
	int GetRequiredStackToCraft() const {return this->zRequiredStackToCraft;}

	void SetCraftingType(const int TYPE) {this->zCraftingType = TYPE;}
	void SetRequiredStackToCraft(const int size) {this->zRequiredStackToCraft = size;}

private:
	int zCraftingType;
	int zRequiredStackToCraft;
};