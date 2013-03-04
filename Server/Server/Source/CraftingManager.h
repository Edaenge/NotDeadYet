#pragma once

#include "Inventory.h"
#include <map>

struct CraftedTypes
{
	unsigned int subType;
	unsigned int type;

	CraftedTypes(unsigned int type, unsigned int subType)
	{
		this->type = type;
		this->subType = subType;
	}
};

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
	/*! Map to Crafted types and required materials and stacks.*/
	std::map<CraftedTypes*, std::map<unsigned int, unsigned int> > zMappedTypesToMaterialsReq;
	std::vector<CraftedTypes*> zCraftTypes;
};