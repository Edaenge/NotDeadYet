#include "MaterialSpawnSet.h"
#include <fstream>
#include <map>

static bool loaded = false;
static std::map<unsigned int, MaterialSpawnSet> materialDrops;


void ImportMaterialDrops(const std::string& filePath)
{
	std::ifstream file(filePath);

	while(file.good())
	{
		std::string curLine;
		std::getline(file, curLine);
		
		// Force lowercase
		for( unsigned int x=0; x<curLine.size(); ++x )
		{
			curLine[x] = (char)tolower(curLine[x]);
		}

		if ( curLine.find("material:") == 0 )
		{
			MaterialSpawnSet::MaterialSpawnInfo mat;
			unsigned int entID = 0;

			// Scan
			if ( sscanf(curLine.c_str(), "material:%u, %u, %f, %f", &entID, &mat.materialID, &mat.chance, &mat.distance) == 4 )
			{
				materialDrops[entID].zMaterials.push_back(mat);
			}
		}
	}

	file.close();
}

const MaterialSpawnSet* GetMaterialSpawnSet(unsigned int EntityTypeID)
{
	if ( !loaded ) 
	{
		ImportMaterialDrops("MaterialSpawn.cfg");
		loaded = true;
	}

	auto it = materialDrops.find(EntityTypeID);
	if ( it != materialDrops.end() )
	{
		return &it->second;
	}

	return 0;
}