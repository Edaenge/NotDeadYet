#include "PlayerConfigReader.h"
#include "MaloW.h"
#include "Safe.h"

static const std::string FILENAME = "PlayerConditionConfig.cfg";

namespace 
{
	static PlayerConfigReader* playerConfig = NULL;
}
PlayerConfigReader* GetPlayerConfig()
{
	return playerConfig;
}

void InitPlayerConfig()
{
	if (playerConfig == NULL)
		playerConfig = new PlayerConfigReader();
}

void FreePlayerConfig()
{
	SAFE_DELETE(playerConfig);
}

PlayerConfigReader::PlayerConfigReader()
{
	if (!ReadFromFile())
	{
		this->zVariables[FULLNESS_MAX] = 100.0f;
		this->zVariables[HUNGER_COEFF] = 0.1f;
		this->zVariables[HUNGER_SPRINT_COEFF] = 0.3f;
		this->zVariables[HYDRATION_MAX] = 100;
		this->zVariables[HYDRATION_COEFF] = 0.14f;
		this->zVariables[HYDRATION_SPRINT_COEFF] = 0.6f;
		this->zVariables[STAMINA_SPRINT_COEFF] = 2.0f;
		this->zVariables[DAMAGE_AT_STARVATON_COEFF] = 20.4f;
		this->zVariables[DAMAGE_AT_THIRST_COEFF] = 20.4f;
		this->zVariables[STAMINA_DECREASE_COEFF_HUNGER] = 0.6f;
		this->zVariables[STAMINA_DECREASE_COEFF_HYDRATION] = 0.8f;
		this->zVariables[STAMINA_DECREASE_BLEEDING_COEF] = 1.2f;
		this->zVariables[HUNGER_DECREASE_BLEEDING_COEFF] = 0.2f;
		this->zVariables[HYDRATION_DECREASE_BLEEDING_COEFF] = 0.3f;
		this->zVariables[HUNGER_FOR_STAMINA_COEFF] = 0.05f;
		this->zVariables[HYDRATION_FOR_STAMINA_COEFF] = 0.1f;
		this->zVariables[UPPER_HUNGER] = 0.7f;
		this->zVariables[LOWER_HUNGER] = 0.3f;
		this->zVariables[UPPER_HYDRATION] = 0.8f;
		this->zVariables[LOWER_HYDRATION] = 0.4f;
		this->zVariables[UPPER_STAMINA] = 0.8f;
		this->zVariables[LOWER_STAMINA] = 0.15f;
		this->zVariables[REGEN_SCALE] = 50.0f;
		this->zVariables[REGEN_HUNGER_ADD] = 0.8f;
		this->zVariables[REGEN_HYDRATION_ADD] = 0.25f;
		this->zVariables[REGEN_STAMINA_ADD] = 0.20f;
		this->zVariables[ENERGY_COEFF] = 0.055f;

		this->WriteToFile();
	}
}

void PlayerConfigReader::WriteToFile()
{
	std::ofstream write;

	write.open(FILENAME);

	std::string key;
	float value;

	auto it_zVariables_end = this->zVariables.end();
	for(auto it = this->zVariables.begin(); it != it_zVariables_end; it++)
	{
		key = it->first;
		value = it->second;
		
		write << key << "=" << value<< "\n";
	}
	write.close();
}

PlayerConfigReader::~PlayerConfigReader()
{
	this->zVariables.clear();
}

bool PlayerConfigReader::ReadFromFile()
{
	std::ifstream read;
	read.open(FILENAME);

	if (!read.is_open())
		return false;

	while (!read.eof())
	{
		std::stringstream ss;
		std::string line;
		std::string key;
		std::string value;

		std::getline(read, line);
		
		if (line.empty())
			continue;

		TrimAndSet(line, key, value);
		
		float fValue;
		ss << value;
		ss >> fValue;
		
		this->zVariables[key] = fValue;
	}

	read.close();

	return true;
}

float PlayerConfigReader::GetVariable( std::string mappedName )
{
	auto it = this->zVariables.find(mappedName);

	if (it != this->zVariables.end())
		return it->second;

	return 1.0f;
}