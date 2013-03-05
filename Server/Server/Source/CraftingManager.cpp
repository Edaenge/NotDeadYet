#include "CraftingManager.h"

CraftingManager::CraftingManager()
{
	CraftedTypes* craft_Bow = new CraftedTypes(ITEM_TYPE_WEAPON_RANGED, ITEM_SUB_TYPE_BOW);
	
	std::map<unsigned int, unsigned int> Bow_Materials;
	Bow_Materials[ITEM_SUB_TYPE_MEDIUM_STICK] = 4;
	Bow_Materials[ITEM_SUB_TYPE_THREAD] = 2;
	this->zMappedTypesToMaterialsReq[craft_Bow] = Bow_Materials;
	this->zCraftTypes.push_back(craft_Bow);

	CraftedTypes* craft_Arrow = new CraftedTypes(ITEM_TYPE_PROJECTILE, ITEM_SUB_TYPE_ARROW);
	std::map<unsigned int, unsigned int> Arrow_Materials;
	Arrow_Materials[ITEM_SUB_TYPE_SMALL_STICK] = 2;
	this->zMappedTypesToMaterialsReq[craft_Arrow] = Arrow_Materials;
	this->zCraftTypes.push_back(craft_Arrow);

	CraftedTypes* craft_Bandage = new CraftedTypes(ITEM_TYPE_BANDAGE, ITEM_SUB_TYPE_BANDAGE_POOR);
	std::map<unsigned int, unsigned int> Bandage_Materials;
	Bandage_Materials[ITEM_SUB_TYPE_DISENFECTANT_LEAF] = 1;
	Bandage_Materials[ITEM_SUB_TYPE_THREAD] = 1;
	this->zMappedTypesToMaterialsReq[craft_Bandage] = Bandage_Materials;
	this->zCraftTypes.push_back(craft_Bandage);

	CraftedTypes* craft_Trap = new CraftedTypes(ITEM_TYPE_MISC, ITEM_SUB_TYPE_REGULAR_TRAP);
	std::map<unsigned int, unsigned int> Trap_Materials;
	Trap_Materials[ITEM_SUB_TYPE_LARGE_STICK] = 8;
	Trap_Materials[ITEM_SUB_TYPE_MEDIUM_STICK] = 10;
	Trap_Materials[ITEM_SUB_TYPE_THREAD] = 4;
	this->zMappedTypesToMaterialsReq[craft_Trap] = Trap_Materials;
	this->zCraftTypes.push_back(craft_Trap);

	CraftedTypes* craft_Campfire = new CraftedTypes(ITEM_TYPE_MISC, ITEM_SUB_TYPE_CAMPFIRE);
	std::map<unsigned int, unsigned int> Campfire_Materials;
	Campfire_Materials[ITEM_SUB_TYPE_LARGE_STICK] = 2;
	Campfire_Materials[ITEM_SUB_TYPE_MEDIUM_STICK] = 3;
	Campfire_Materials[ITEM_SUB_TYPE_SMALL_STICK] = 4;
	this->zMappedTypesToMaterialsReq[craft_Campfire] = Campfire_Materials;
	this->zCraftTypes.push_back(craft_Campfire);
}

CraftingManager::~CraftingManager()
{

}

bool CraftingManager::Craft( Inventory* inv, CraftedTypes* types, std::map<Item*, unsigned int>& mapOut)
{
	bool found = false;
	for (auto it = this->zCraftTypes.begin(); it != this->zCraftTypes.end() && !found; it++)
	{
		CraftedTypes* temp = (*it);
		if (temp->type == types->type && temp->subType == types->subType)
		{
			types = temp;
			found = true;
		}
	}

	auto map_It = this->zMappedTypesToMaterialsReq.find(types);

	if (map_It != this->zMappedTypesToMaterialsReq.end())
	{
		unsigned int material_SubType;
		unsigned int material_Stacks;
		Item* item = NULL;

		auto material_iterator = map_It->second;
		for (auto it = material_iterator.begin(); it != material_iterator.end(); it++)
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