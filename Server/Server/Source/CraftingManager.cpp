#include "CraftingManager.h"
#include "CraftingReader.h"

CraftingManager::CraftingManager()
{

}

CraftingManager::~CraftingManager()
{

}

bool CraftingManager::Craft(Inventory* inv, CraftedTypes* types, std::map<Item*, unsigned int>& mapOut)
{
	auto map_It = GetCraftingRecipes()->GetMaterialReq(types);

	if (map_It != NULL)
	{
		unsigned int material_SubType;
		unsigned int material_Stacks;
		Item* item = NULL;

		for (auto it = map_It->begin(); it != map_It->end(); it++)
		{
			material_SubType = it->first;
			material_Stacks = it->second;

			item = inv->SearchAndGetItemFromType(ITEM_TYPE_MATERIAL, material_SubType);

			if (!item)
			{
				mapOut.clear();
				return false;
			}
			int newStacks = item->GetStackSize() - material_Stacks;
			if (newStacks < 0)
				return false;

			mapOut[item] = material_Stacks;
		}

		return true;
	}

	return false;
}