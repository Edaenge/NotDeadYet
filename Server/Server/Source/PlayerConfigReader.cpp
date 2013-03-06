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
		this->zVariables[HYDRATION_COEFF] = 0.12f;
		this->zVariables[HYDRATION_SPRINT_COEFF] = 0.6f;
		this->zVariables[STAMINA_SPRINT_COEFF] = 2.0f;
		this->zVariables[DAMAGE_AT_STARVATON_COEFF] = 0.1f;
		this->zVariables[DAMAGE_AT_THIRST_COEFF] = 0.4f;
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

	for(auto it = this->zVariables.begin(); it != this->zVariables.end(); it++)
	{
		key = it->first;
		value = it->second;
		
		write << key << "=" << value<< "\n";
	}
	write.close();
}

PlayerConfigReader::~PlayerConfigReader()
{

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
		//this->InterpCommand(key, fValue);
		this->zVariables[key] = fValue;
	}
	return true;
}

float PlayerConfigReader::GetVariable( std::string mappedName )
{
	auto it = this->zVariables.find(mappedName);

	if (it != this->zVariables.end())
		return it->second;

	return 1.0f;
}

//void PlayerConfigReader::InterpCommand(std::string key, float value)
//{
//	if (key == FULLNESS_MAX)
//		zFullnessMax = value;
//	else if (key == HUNGER_COEFF)
//		zHungerCof = value;
//	else if (key == HUNGER_SPRINT_COEFF)
//		zHungerSprintingCof = value;
//	else if (key == HYDRATION_MAX)
//		zHydrationMax = value;
//	else if (key == HYDRATION_COEFF)
//		zHydrationCof = value;
//	else if (key == HYDRATION_SPRINT_COEFF)
//		zHydrationSprintingCof = value;
//	else if (key == STAMINA_SPRINT_COEFF)
//		zStaminaSprintingCof = value;
//	else if (key == DAMAGE_AT_STARVATON_COEFF)
//		zDamageAtStarvationCof = value;
//	else if (key == DAMAGE_AT_THIRST_COEFF)
//		zDamageAtThirstCof = value;
//	else if (key == STAMINA_DECREASE_COEFF_HUNGER)
//		zStaminaDecreaseCofWithHunger = value;
//	else if (key == STAMINA_DECREASE_COEFF_HYDRATION)
//		zStaminaDecreaseCofWithHydration = value;
//	else if (key == STAMINA_DECREASE_BLEEDING_COEF)
//		zStaminaDecreaseWithBleedingCof = value;
//	else if (key == HUNGER_DECREASE_BLEEDING_COEFF)
//		zHungerDecreaseWithBleedingCof = value;
//	else if (key == HYDRATION_DECREASE_BLEEDING_COEFF)
//		zHydrationDecreaseWithBleedingCof = value;
//	else if (key == HUNGER_FOR_STAMINA_COEFF)
//		zHungerForStaminaCof = value;
//	else if (key == HYDRATION_FOR_STAMINA_COEFF)
//		zHydrationForStaminaCof = value;
//	else if (key == UPPER_HUNGER)
//		zUpperHunger = value;
//	else if (key == LOWER_HUNGER)
//		zLowerHunger = value;
//	else if (key == UPPER_HYDRATION)
//		zUpperHydration = value;
//	else if (key == LOWER_HYDRATION)
//		zLowerHydration = value;
//	else if (key == UPPER_STAMINA)
//		zUpperStamina = value;
//	else if (key == LOWER_STAMINA)
//		zLowerStamina = value;
//	else if (key == REGEN_SCALE)
//		zRegenerationScale = value;
//	else if (key == REGEN_HUNGER_ADD)
//		zRegenerationHungerAddition = value;
//	else if (key == REGEN_HYDRATION_ADD)
//		zRegenerationHydrationAddition = value;
//	else if (key == REGEN_STAMINA_ADD)
//		zRegenerationStaminaAddition = value;
//	else if (key == ENERGY_COEFF)
//		zEnergyCoeff = value;
//	
//}