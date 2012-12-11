/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Inventory/Item.h"
#include <vector>
class Inventory
{
public:
	Inventory();
	virtual ~Inventory();
	/*! Search for the Item in and returns its position*/
	int Search(const unsigned int ID);
	/*! Adds an item to the array*/
	bool AddItem(Item* item);
	/*! Returns the item at the position */
	Item* GetItem(const unsigned int position);
	/*! Returns the Items*/
	const std::vector<Item*>& GetItems();
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(const unsigned int ID);
	/*! Removes the item*/
	bool RemoveItem(const unsigned int position);
	bool RemoveItem(Item* item);

private:
	//void Sort();
private:
	std::vector<Item*> zItems;
	std::vector<bool> zInventorySlotUsed;
	unsigned int zSlotsAvailable;
	unsigned int zInventoryCap;
	unsigned int zWeightTotal;
};