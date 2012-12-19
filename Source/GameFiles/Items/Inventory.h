/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/RangedWeapon.h"
#include "GameFiles/Items/MeleeWeapon.h"
#include "GameFiles/Items/Gear.h"
#include "GameFiles/Items/Food.h"
#include <vector>
#include "Array.h"

/*! Class That keeps track of the inventory data*/
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
	std::vector<Item*> GetItems() const;
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(const unsigned int ID) const;
	/*! Removes the item*/
	bool RemoveItem(const unsigned int position);
	bool RemoveItem(Item* item);
	/*! Returns blocked slots*/
	std::vector<bool> GetBlockedSlots() const;
	/*! Get the status of the slot at Position*/
	bool GetBlockedSlot(unsigned int position) const;
	/*! Returns the Inventory Capacity == Max Weight allowed*/
	int GetInventoryCapacity() const;

private:
	/*! Array of items*/
	std::vector<Item*> zItems;
	/*! Array to keep track of Gui Representation*/
	std::vector<bool> zInventorySlotBlocked;

	/*! Nr of slots available in inventory == zInventorySlotBlocked array*/
	unsigned int zSlotsAvailable;

	/*! Max nr of slots in The zInventorySlotBlocked array*/
	unsigned int zInventoryCap;

	/*! Nr of slots Occupied == Current Weight*/
	unsigned int zWeightTotal;
};
/* Gui Draws the Items in zItems and the blocked slots zInventorySlotBlocked
So Weight = number of Items + blocked slots*/