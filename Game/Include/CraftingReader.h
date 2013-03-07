#pragma once

#include "FileReader.h"
#include <map>
#include <vector>

struct CraftedTypes
{
	unsigned int subType;
	unsigned int type;
	std::string filePath;

	CraftedTypes()
	{
		type = 100;
		subType = 100;
		filePath = "Media/Models/Unknown.png";
	}
	CraftedTypes(unsigned int type, unsigned int subType, const std::string& filePath)
	{
		this->type = type;
		this->subType = subType;
		this->filePath = filePath;
	}
};

class CraftingReader : public FileReader
{
public:
	CraftingReader();
	virtual ~CraftingReader();

	bool ReadFromFile();
	void WriteToFile();
	void InterpCommand(std::string value, std::string key, CraftedTypes*& ct);
	const std::map<unsigned int, unsigned int>* GetMaterialReq(CraftedTypes* types);
	std::vector<CraftedTypes*> GetCraftableItemTypes(unsigned int);
private:
	std::map<CraftedTypes*, std::map<unsigned int, unsigned int>> zMappedTypesToMaterialsReq;
	std::vector<CraftedTypes*> zCraftingList;
};

void FreeCraftingRecipes();
void InitCraftingRecipes();
CraftingReader* GetCraftingRecipes();