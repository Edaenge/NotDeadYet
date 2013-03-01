#pragma once

#include "FileReader.h"
#include <map>

static const std::string FULLNESS_MAX						= "FULLNESS_MAX";
static const std::string HUNGER_COEFF						= "HUNGER_COEFF";
static const std::string HUNGER_SPRINT_COEFF				= "HUNGER_SPRINT_COEFF";
static const std::string HYDRATION_MAX						= "HYDRATION_MAX";
static const std::string HYDRATION_COEFF					= "HYDRATION_COEFF";
static const std::string HYDRATION_SPRINT_COEFF				= "HYDRATION_SPRINT_COEFF";
static const std::string STAMINA_SPRINT_COEFF				= "STAMINA_SPRINT_COEFF";
static const std::string DAMAGE_AT_STARVATON_COEFF			= "DAMAGE_AT_STARVATON_COEFF";
static const std::string DAMAGE_AT_THIRST_COEFF				= "DAMAGE_AT_THIRST_COEFF";
static const std::string STAMINA_DECREASE_COEFF_HUNGER		= "STAMINA_DECREASE_COEFF_HUNGER";
static const std::string STAMINA_DECREASE_COEFF_HYDRATION	= "STAMINA_DECREASE_COEFF_HYDRATION";
static const std::string STAMINA_DECREASE_BLEEDING_COEF		= "STAMINA_DECREASE_BLEEDING_COEF";
static const std::string HUNGER_DECREASE_BLEEDING_COEFF		= "HUNGER_DECREASE_BLEEDING_COEFF";
static const std::string HYDRATION_DECREASE_BLEEDING_COEFF	= "HYDRATION_DECREASE_BLEEDING_COEFF";
static const std::string HUNGER_FOR_STAMINA_COEFF			= "HUNGER_FOR_STAMINA_COEFF";
static const std::string HYDRATION_FOR_STAMINA_COEFF		= "HYDRATION_FOR_STAMINA_COEFF";
static const std::string UPPER_HUNGER						= "UPPER_HUNGER";
static const std::string LOWER_HUNGER						= "LOWER_HUNGER";
static const std::string UPPER_HYDRATION					= "UPPER_HYDRATION";
static const std::string LOWER_HYDRATION					= "LOWER_HYDRATION";
static const std::string UPPER_STAMINA						= "UPPER_STAMINA";
static const std::string LOWER_STAMINA						= "LOWER_STAMINA";
static const std::string REGEN_SCALE						= "REGEN_SCALE";
static const std::string REGEN_HUNGER_ADD					= "REGEN_HUNGER_ADD";
static const std::string REGEN_HYDRATION_ADD				= "REGEN_HYDRATION_ADD";
static const std::string REGEN_STAMINA_ADD					= "REGEN_STAMINA_ADD";
static const std::string ENERGY_COEFF						= "ENERGY_COEFF";

class PlayerConfigReader : public FileReader
{
public:
	PlayerConfigReader();
	virtual ~PlayerConfigReader();
	bool ReadFromFile();

	float GetVariable(std::string mappedName);
private:
	std::map<std::string, float> zVariables;
};

PlayerConfigReader* GetPlayerConfig();

void InitPlayerConfig();
void FreePlayerConfig();