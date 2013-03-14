/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include <string>
#include "NetworkMessageConverter.h"

static const enum M_ITEM_TYPE
{
	ITEM_TYPE_WEAPON_RANGED,
	ITEM_TYPE_WEAPON_MELEE,
	ITEM_TYPE_PROJECTILE,
	ITEM_TYPE_CONTAINER,
	ITEM_TYPE_FOOD,
	ITEM_TYPE_MATERIAL,
	ITEM_TYPE_GEAR,
	ITEM_TYPE_BANDAGE,
	ITEM_TYPE_MISC
};
/*! Abstract class for Items*/
class Item
{
public:
	Item();
	Item(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType);
	Item(const unsigned int itemType, const unsigned int itemSubType);
	virtual ~Item(){}

	/*! Returns Item ID*/
	inline unsigned int GetID() const {return this->zID;}

	/*! Returns the Item Weight*/
	inline float GetWeight() const {return this->zWeight;}

	/*! Returns the Item Description*/
	inline const std::string& GetItemName() const {return this->zItemName;}

	/*! Returns the Item Type*/
	inline unsigned int GetItemType() const {return this->zItemType;}
	inline unsigned int GetItemSubType() const {return this->zItemSubType;}

	/*! Returns the Item Description*/
	inline const std::string& GetItemDescription() const {return this->zItemDescription;}

	/*! Returns number of stacks the item has.*/
	inline unsigned int GetStackSize() const {return this->zStacks;}
	inline unsigned int GetSlotSize() const {return this->zSlotSize;}
	inline const std::string& GetIconPath() const {return this->zIconPath;}

	inline void SetIconPath(const std::string& path) {this->zIconPath = path;}

	/*! Sets the Item ID*/
	inline void SetItemID(const unsigned int id) {this->zID = id;}

	/*! Sets the Item Weight*/
	inline void SetItemWeight(const float weight) {this->zWeight = weight;}

	/*! Sets the Item Name*/
	inline void SetItemName(const std::string& name) {this->zItemName = name;}

	/*! Sets The Item Type*/
	inline void SetItemType(const unsigned int type) {this->zItemType = type;}
	inline void SetItemSubType(const unsigned int SubType) {this->zItemSubType = SubType;}

	/*! Sets the Item Description*/
	inline void SetItemDescription(const std::string& description) {this->zItemDescription = description;}

	/*! Sets the mesh model*/
	inline void SetModel(const std::string& model) {this->zMeshModel = model;}

	/*! Sets the Item Stack Size.*/
	inline void SetStackSize(const unsigned int size) {this->zStacks = size;}
	inline void SetSlotSize(const unsigned int size) {this->zSlotSize = size;}
	inline void SetStacking(bool value) {this->zStacking = value;}
	inline void IncreaseStackSize(const unsigned int size) {this->zStacks += size;}
	inline void DecreaseStackSize(const unsigned int size) {this->zStacks -= size;}

	inline bool GetStacking() const {return this->zStacking;}
	inline const std::string& GetModel() const {return this->zMeshModel;}

	virtual bool Use() = 0;
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);

protected:
	void GenerateID();

protected:
	unsigned int zStacks;
	unsigned int zID;
	float zWeight;
	unsigned int zSlotSize;
	unsigned int zItemType;
	unsigned int zItemSubType;
	std::string zItemDescription;
	std::string zIconPath;
	std::string zItemName;
	std::string zMeshModel;
	bool zStacking;

private:

};