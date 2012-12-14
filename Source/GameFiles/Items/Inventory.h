/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Item.h"
#include <vector>
#include "Array.h"
class Inventory
{
public:
	Inventory();
	virtual ~Inventory();
	/*! Search for the Item in and returns its position*/
	int Search(const unsigned int ID) const;
	/*! Adds an item to the array*/
	bool AddItem(Item* item);
	/*! Returns the item at the position */
	Item* GetItem(const unsigned int position) const;
	/*! Returns the Items*/
	MaloW::Array<Item*>* GetItems() const;
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(const unsigned int ID) const;
	/*! Removes the item*/
	bool RemoveItem(const unsigned int position);
	bool RemoveItem(Item* item);
	/*! Returns blocked slots*/
	MaloW::Array<bool>* GetBlockedSlots() const;
	/*! Get the status of the slot at Position*/
	bool GetBlockedSlot(unsigned int position) const;
	/*! Returns the Inventory Capacity == Max Weight allowed*/
	int GetInventoryCapacity() const;
private:
	/*! Places the item to be removed At the last position*/
	bool MoveAndRemove(const unsigned int position);
	unsigned int Move(const unsigned int position);
private:
	/*! Array of items*/
	MaloW::Array<Item*>* zItems;
	/*! Array to keep track of Gui Representation*/
	MaloW::Array<bool>* zInventorySlotBlocked;

	/*! Nr of slots available in inventory == zInventorySlotBlocked array*/
	unsigned int zSlotsAvailable;

	/*! Max nr of slots in The zInventorySlotBlocked array*/
	unsigned int zInventoryCap;

	/*! Nr of slots Occupied == Current Weight*/
	unsigned int zWeightTotal;
};
/* Gui Draws the Items in zItems and the blocked slots zInventorySlotBlocked
So Weight = number of Items + blocked slots*/