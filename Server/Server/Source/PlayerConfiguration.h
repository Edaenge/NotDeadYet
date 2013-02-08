#pragma once

#include<fstream>



class PlayerConfiguration
{

	public:
		PlayerConfiguration();
		bool SetValuesFromFile();
		

	public:
		float	zFullnessMax;
		float	zHungerCof;
		float	zHungerSprintingCof;
		float	zHydrationMax;
		float	zHydrationCof;
		float	zHydrationSprintingCof;

		//bool zHydrationChanged;
		//bool zHungerChanged;

		float zDamageAtStarvationCof;
		float zDamageAtThirstCof;

	
		float zStaminaDecreaseCofWithHunger;
		float zStaminaDecreaseCofWithHydration;

		float zStaminaDecreaseWithBleedingCof;
		float zHungerDecreaseWithBleedingCof;
		float zHydrationDecreaseWithBleedingCof;

		float zHungerForStaminaCof;
		float zHydrationForStaminaCof;

		float zUpperHunger;
		float zLowerHunger;
		float zUpperHydration;
		float zLowerHydration;
		float zUpperStamina;
		float zLowerStamina;

		float zRegenerationScale;
		float zRegenerationHungerAddition;
		float zRegenerationHydrationAddition;
		float zRegenerationStaminaAddition;




};

const PlayerConfiguration& GetPlayerConfiguration();