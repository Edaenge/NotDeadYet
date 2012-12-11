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
	int Search(unsigned int ID);
	/*! Adds an item to the array*/
	bool AddItem(Item* item);
	/*! Returns the item at the position */
	Item* GetItem(unsigned int position);
	/*! Returns the Items*/
	std::vector<Item*> GetItems();
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(unsigned int ID);
	/*! Removes the item*/
	bool RemoveItem(unsigned int position);
	bool RemoveItem(Item* item);

private:
	//void Sort();
private:
	std::vector<Item*> zItems;
	unsigned int zItemCap;
	unsigned int zInventoryCap;
	unsigned int zWeightTotal;
};