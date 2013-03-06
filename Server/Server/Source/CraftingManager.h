#pragma once

#include "Inventory.h"
#include "CraftingReader.h"
#include <map>



/*********************************************************
*														 *
*	 Class that handles all logic concerning crafting    *
*														 *
**********************************************************/

class CraftingManager
{
public:
	CraftingManager();
	virtual ~CraftingManager();
	/*! Returns result of crafting, mapOut returns items used and amount of stacks used.*/
	bool Craft(Inventory* inv, CraftedTypes* types, std::map<Item*, unsigned int>& mapOut);
private:
};