#pragma once

struct Damage
{
	float slashing;
	float blunt;
	float fallingDamage;
	float piercing;
	float fogDamage;
	float internalDamage;
	Damage()
	{
		slashing = .0f;
		blunt	= .0f;
		piercing = .0f;
		fallingDamage = .0f;
		fogDamage = .0f;
		internalDamage = .0f;
	}

	float GetTotal() const
	{
		return slashing + blunt + piercing + fallingDamage + fogDamage + internalDamage;
	}

	float GetBleedFactor() const
	{
		// TODO: Balance
		return (piercing + slashing) / GetTotal();
	}
};