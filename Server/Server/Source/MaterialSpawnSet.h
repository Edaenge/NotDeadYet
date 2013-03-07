/*
Material Spawn List. Manages the the randomization data of material spawning
By: Alexivan 2013-03-07
*/

#pragma once

#include <vector>
#include <string>


struct MaterialSpawnSet
{
	struct MaterialSpawnInfo
	{
		unsigned int materialID;
		float chance;
		float distance;
	};

	std::vector<MaterialSpawnInfo> zMaterials;
};

void ImportMaterialDrops(const std::string& filePath);
const MaterialSpawnSet* GetMaterialSpawnSet(unsigned int EntityTypeID);