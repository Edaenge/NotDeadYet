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
	ITEM_TYPE_BANDAGE
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
	inline unsigned int GetWeight() const {return this->zWeight;}
	/*! Returns the Item Description*/
	std::string GetItemName() const {return this->zItemName;}
	/*! Returns the Item Type*/
	unsigned int GetItemType() const {return this->zItemType;}
	unsigned int GetItemSubType() const {return this->zItemSubType;}
	/*! Returns the Item Description*/
	std::string GetItemDescription() const {return this->zItemDescription;}
	/*! Returns number of stacks the item has.*/
	unsigned int GetStackSize() const {return this->zStacks;}
	unsigned int GetSlotSize() const {return this->zSlotSize;}
	std::string GetIconPath() const {return this->zIconPath;}

	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	/*! Sets the Item ID*/
	void SetItemID(const unsigned int id) {this->zID = id;}
	/*! Sets the Item Weight*/
	void SetItemWeight(const unsigned int weight) {this->zWeight = weight;}
	/*! Sets the Item Name*/
	void SetItemName(const std::string& name) {this->zItemName = name;}
	/*! Sets The Item Type*/
	void SetItemType(const unsigned int type) {this->zItemType = type;}
	void SetItemSubType(const unsigned int SubType) {this->zItemSubType = SubType;}
	/*! Sets the Item Description*/
	void SetItemDescription(const std::string& description) {this->zItemDescription = description;}
	/*! Sets the mesh model*/
	void SetModel(const std::string& model) {this->zMeshModel = model;}
	/*! Sets the Item Stack Size.*/
	void SetStackSize(const unsigned int size) {this->zStacks = size;}
	void SetSlotSize(const unsigned int size) {this->zSlotSize = size;}
	void SetStacking(bool value) {this->zStacking = value;}
	void IncreaseStackSize(const unsigned int size) {this->zStacks += size;}
	void DecreaseStackSize(const unsigned int size) {this->zStacks -= size;}

	bool GetStacking() const {return this->zStacking;}
	const std::string& GetModel() const {return this->zMeshModel;}

	virtual bool Use() = 0;
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);

protected:
	void GenerateID();

protected:
	unsigned int zStacks;
	unsigned int zID;
	int zWeight;
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