/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include <string>
static const enum M_ITEM_TYPE
{
	ITEM_TYPE_WEAPON_RANGED_BOW,
	ITEM_TYPE_WEAPON_RANGED_ROCK,
	ITEM_TYPE_WEAPON_MELEE_AXE,
	ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE,
	ITEM_TYPE_GEAR_HEAD,
	ITEM_TYPE_GEAR_CHEST,
	ITEM_TYPE_GEAR_LEGS,
	ITEM_TYPE_GEAR_BOOTS,
	ITEM_TYPE_CONTAINER_CANTEEN,
	ITEM_TYPE_CONTAINER_WATER_BOTTLE,
	ITEM_TYPE_FOOD_DEER_MEAT,
	ITEM_TYPE_FOOD_WOLF_MEAT,
	ITEM_TYPE_PROJECTILE_ARROW,
	ITEM_TYPE_MATERIAL_SMALL_STICK,
	ITEM_TYPE_MATERIAL_MEDIUM_STICK,
	ITEM_TYPE_MATERIAL_LARGE_STICK,
	ITEM_TYPE_MATERIAL_THREAD
};

/*! Abstract class for Items*/
class Item
{
public:
	Item(const unsigned int id, const unsigned int itemType);
	virtual ~Item(){}
	/*! Returns Item ID*/
	inline int GetID() const {return this->zID;}
	/*! Returns the Item Weight*/
	inline unsigned int GetWeight() const {return this->zWeight;}
	/*! Returns the Item Description*/
	std::string GetItemName() const {return this->zItemName;}
	/*! Returns the Item Type*/
	unsigned int GetItemType() const {return this->zItemType;}
	/*! Returns the Item Description*/
	std::string GetItemDescription() const {return this->zItemDescription;}
	/*! Returns number of stacks the item has.*/
	unsigned int GetStackSize() const {return this->zStacks;}
	
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
	/*! Sets the Item Description*/
	void SetItemDescription(const std::string& description) {this->zItemDescription = description;}
	/*! Sets the Item Stack Size.*/
	void SetStackSize(const unsigned int size) {this->zStacks = size;}
	void IncreaseStackSize(const unsigned int size) {this->zStacks += size;}
	void DecreaseStackSize(const unsigned int size) {this->zStacks -= size;}
	bool GetStacking() const {return this->zStacking;}
	void SetStacking(bool value) {this->zStacking = value;}
	virtual bool Use() = 0;

private:
	void InitValues(const int id = -1, const unsigned int itemType = 0, const unsigned int weight = 0, 
		const std::string& name = "Unknown", const std::string& description = "Unknown", const std::string iconPath = "Unknown");
protected:
	unsigned int zStacks;
	unsigned int zID;
	unsigned int zWeight;
	std::string zItemName;
	unsigned int zItemType;
	std::string zItemDescription;
	std::string zIconPath;
	bool zStacking;
};