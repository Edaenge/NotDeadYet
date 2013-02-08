#include "PlayerConfiguration.h"
#include <iostream>

PlayerConfiguration::PlayerConfiguration()
{
	if(!this->SetValuesFromFile() ) 
	{
		//Values could not be retrieved from file, setting default values.
		this->zFullnessMax = 100.0f;
		this->zHungerCof = 0.01f;
		this->zHungerSprintingCof = 0.02f;
		this->zHydrationMax = 100.0f;
		this->zHydrationCof = 0.01f;
		this->zHydrationSprintingCof = 0.04f;
		this->zDamageAtStarvationCof = 0.01f;
		this->zDamageAtThirstCof = 0.03f;
		this->zStaminaDecreaseCofWithHunger = 0.01f;
		this->zStaminaDecreaseCofWithHydration = 0.02f;
		this->zStaminaDecreaseWithBleedingCof = 0.02f;
		this->zHungerDecreaseWithBleedingCof = 0.01f;
		this->zHydrationDecreaseWithBleedingCof = 0.02f;
		this->zHungerForStaminaCof = 0.03f;
		this->zHydrationForStaminaCof = 0.05f;
		this->zUpperHunger = 0.7f;
		this->zLowerHunger = 0.3f;
		this->zUpperHydration = 0.8f;
		this->zLowerHydration = 0.4f;
		this->zUpperStamina = 0.8f;
		this->zLowerStamina = 0.15f;
		this->zRegenerationScale = 5.0f;
		this->zRegenerationHungerAddition = 1.0f;
		this->zRegenerationHydrationAddition = 0.4f;
		this->zRegenerationStaminaAddition = 0.5f;

	}
}

bool PlayerConfiguration::SetValuesFromFile()
{
	float numberFromFile = 0;
	bool valuesRetrieved = false;
	char characters[16];

	std::ifstream infile("playerConditionVariables.txt");
	if(infile.good())
	{
		//scanf_s(characters);
		//Max level of fullness
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFullnessMax = numberFromFile;

		//Hunger cof
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHungerCof = numberFromFile;
		
		//Hunger sprinting cof
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHungerSprintingCof = numberFromFile;

		//Max hydration
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHydrationMax = numberFromFile;

		//Hydration cof
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHydrationCof = numberFromFile;

		//Hydration sprinting cof
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHydrationSprintingCof = numberFromFile;

		//Damage cof when you starve 
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zDamageAtStarvationCof = numberFromFile;

		//Damage cof when you are too thirsty
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zDamageAtThirstCof = numberFromFile;

		//Stamina decrease cof with hunger
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zStaminaDecreaseCofWithHunger = numberFromFile;

		//Stamina decrease cof with hydration
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zStaminaDecreaseCofWithHydration = numberFromFile;

		//Stamina decrease cof with bleeding
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zStaminaDecreaseWithBleedingCof = numberFromFile;

		//Hunger decrease cof with bleeding
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHungerDecreaseWithBleedingCof = numberFromFile;
		
		//Hydratopm decrease cof with bleeding
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHydrationDecreaseWithBleedingCof = numberFromFile;

		//Hunger for stamina cof
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHungerForStaminaCof = numberFromFile;

		//Hydration for stamina cof
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zHydrationForStaminaCof = numberFromFile;

		//Upper hunger
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zUpperHunger= numberFromFile;

		//Lower hunger
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zLowerHunger = numberFromFile;

		//Upper hydration
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zUpperHydration = numberFromFile;

		//Lower hydration
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zLowerHydration = numberFromFile;

		//Upper Stamina
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zUpperStamina = numberFromFile;

		//Lower Stamina
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zLowerStamina = numberFromFile;

		//Scale of regenerating health
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zRegenerationScale = numberFromFile;

		//Regeneration at good hunger
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zRegenerationHungerAddition = numberFromFile;

		//Regeneration at good hydration
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zRegenerationHydrationAddition = numberFromFile;

		//Regeneration at good stamina
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zRegenerationStaminaAddition = numberFromFile;

		valuesRetrieved = true;
	}

	if(infile.is_open())
	{
		infile.close();
	}

	return valuesRetrieved;


}

const PlayerConfiguration& GetPlayerConfiguration()
{
	static PlayerConfiguration config;
	
	return config;
}