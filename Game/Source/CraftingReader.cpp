#include "CraftingReader.h"
#include "Safe.h"
#include "Material.h"
#include "Bandage.h"
#include "RangedWeapon.h"
#include "Misc.h"
#include "Projectile.h"

namespace 
{
	static CraftingReader* craftingRecipe = NULL;
}

void FreeCraftingRecipes()
{
	SAFE_DELETE(craftingRecipe);
}

void InitCraftingRecipes()
{
	if (craftingRecipe)
		delete craftingRecipe;

	craftingRecipe = new CraftingReader();
}

CraftingReader* GetCraftingRecipes()
{
	return craftingRecipe;
}

static const std::string FILENAME		= "CraftingRecipes.cfg";

static const std::string RECIPE			= "#RECIPE";
static const std::string CRAFTING_TYPE	= "CRAFTING_TYPE";
static const std::string CRAFTING_PATH	= "CRAFTING_PATH";
static const std::string MATERIAL_REQ	= "MATERIAL_REQ";
static const std::string END			= "#END";

CraftingReader::CraftingReader()
{
	if(!this->ReadFromFile())
	{
		CraftedTypes* craft_Bow = new CraftedTypes(ITEM_TYPE_WEAPON_RANGED, ITEM_SUB_TYPE_BOW, "Media/Icons/Bow_Icon_Temp.png");

		std::map<unsigned int, unsigned int> Bow_Materials;
		Bow_Materials[ITEM_SUB_TYPE_MEDIUM_STICK] = 4;
		Bow_Materials[ITEM_SUB_TYPE_THREAD] = 2;
		this->zMappedTypesToMaterialsReq[craft_Bow] = Bow_Materials;
		this->zCraftingList.push_back(craft_Bow);

		CraftedTypes* craft_Arrow = new CraftedTypes(ITEM_TYPE_PROJECTILE, ITEM_SUB_TYPE_ARROW, "Media/Icons/Arrow_Icon_Temp.png");
		std::map<unsigned int, unsigned int> Arrow_Materials;
		Arrow_Materials[ITEM_SUB_TYPE_SMALL_STICK] = 2;
		this->zMappedTypesToMaterialsReq[craft_Arrow] = Arrow_Materials;
		this->zCraftingList.push_back(craft_Arrow);

		CraftedTypes* craft_Bandage = new CraftedTypes(ITEM_TYPE_BANDAGE, ITEM_SUB_TYPE_BANDAGE_POOR, "Media/Icons/Poor_Bandage_Icon_Temp.png");
		std::map<unsigned int, unsigned int> Bandage_Materials;
		Bandage_Materials[ITEM_SUB_TYPE_DISENFECTANT_LEAF] = 1;
		Bandage_Materials[ITEM_SUB_TYPE_THREAD] = 1;
		this->zMappedTypesToMaterialsReq[craft_Bandage] = Bandage_Materials;
		this->zCraftingList.push_back(craft_Bandage);

		CraftedTypes* craft_Trap = new CraftedTypes(ITEM_TYPE_MISC, ITEM_SUB_TYPE_REGULAR_TRAP, "Media/Icons/Trap_Icon_Temp.png");
		std::map<unsigned int, unsigned int> Trap_Materials;
		Trap_Materials[ITEM_SUB_TYPE_LARGE_STICK] = 8;
		Trap_Materials[ITEM_SUB_TYPE_MEDIUM_STICK] = 10;
		Trap_Materials[ITEM_SUB_TYPE_THREAD] = 4;
		this->zMappedTypesToMaterialsReq[craft_Trap] = Trap_Materials;
		this->zCraftingList.push_back(craft_Trap);

		CraftedTypes* craft_Campfire = new CraftedTypes(ITEM_TYPE_MISC, ITEM_SUB_TYPE_CAMPFIRE, "Media/Icons/Campfire_Icon_Temp.png");
		std::map<unsigned int, unsigned int> Campfire_Materials;
		Campfire_Materials[ITEM_SUB_TYPE_LARGE_STICK] = 2;
		Campfire_Materials[ITEM_SUB_TYPE_MEDIUM_STICK] = 3;
		Campfire_Materials[ITEM_SUB_TYPE_SMALL_STICK] = 4;
		this->zMappedTypesToMaterialsReq[craft_Campfire] = Campfire_Materials;
		this->zCraftingList.push_back(craft_Campfire);

		this->WriteToFile();
	}
}

CraftingReader::~CraftingReader()
{
	auto it_end = this->zMappedTypesToMaterialsReq.end();
	for (auto it = this->zMappedTypesToMaterialsReq.begin(); it != it_end; it++)
	{
		CraftedTypes* temp = it->first;
		SAFE_DELETE(temp);
	}
	this->zMappedTypesToMaterialsReq.clear();
	this->zCraftingList.clear();
}

bool CraftingReader::ReadFromFile()
{
	std::ifstream read;

	read.open(FILENAME);

	if (!read.is_open())
		return false;

	while (!read.eof())
	{
		std::string line;
		std::string key;
		std::string value;

		std::getline(read, line);

		if (line.empty())
			continue;

		if (line == RECIPE)
		{
			CraftedTypes* ct = new CraftedTypes();
			while (!read.eof() && line != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				if (line != END)
					InterpCommand(value, key, ct);
			}
		}

	}
	return true;
}

void CraftingReader::WriteToFile()
{
	std::ofstream write;
	
	write.open(FILENAME);

	std::string key;

	for (auto it = this->zMappedTypesToMaterialsReq.begin(); it != this->zMappedTypesToMaterialsReq.end(); it++)
	{
		write << RECIPE << "\n";

		write << CRAFTING_TYPE << "=" << it->first->type << "," << it->first->subType << "\n";

		write << CRAFTING_PATH << "=" << it->first->filePath << "\n";

		for (auto it_m = it->second.begin(); it_m != it->second.end(); it_m++)
		{
			write << MATERIAL_REQ << "=" << it_m->first << "," << it_m->second << "\n";
		}

		write << END << "\n \n";
	}
	
}

void CraftingReader::InterpCommand(std::string value, std::string key, CraftedTypes*& ct)
{
	if (key == CRAFTING_TYPE)
	{
		char a[10];
		char b[10];

		sscanf_s(value.c_str(), "%[^','], %[^','], %s", a, sizeof(a), b, sizeof(b));

		int type = atoi(a);
		int subType = atoi(b);
		ct->type = type;
		ct->subType = subType;

		std::map<unsigned int, unsigned int> map;
		this->zMappedTypesToMaterialsReq[ct] = map;

		this->zCraftingList.push_back(ct);
	}
	else if (key == CRAFTING_PATH)
	{
		ct->filePath = value;
	}
	else if (key == MATERIAL_REQ)
	{
		char a[10];
		char b[10];

		sscanf_s(value.c_str(), "%[^','], %[^','], %s", a, sizeof(a), b, sizeof(b));

		int subType = atoi(a);
		int stacks = atoi(b);

		auto it = this->zMappedTypesToMaterialsReq.find(ct);

		if (it != this->zMappedTypesToMaterialsReq.end())
		{
			std::map<unsigned int, unsigned int> matMap = it->second;
			matMap[subType] = stacks;

			this->zMappedTypesToMaterialsReq[ct] = matMap;
		}
	}
}

const std::map<unsigned int, unsigned int>* CraftingReader::GetMaterialReq(CraftedTypes* types)
{
	auto it_end = this->zMappedTypesToMaterialsReq.end();
	for (auto it = this->zMappedTypesToMaterialsReq.begin(); it != it_end; it++)
	{
		if (it->first->type == types->type && it->first->subType == types->subType)
		{
			return &it->second;
		}
	}
	return NULL;
}

std::vector<CraftedTypes*> CraftingReader::GetCraftableItemTypes( unsigned int )
{
	return this->zCraftingList;
}