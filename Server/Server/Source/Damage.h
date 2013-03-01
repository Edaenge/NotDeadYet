#pragma once

struct Damage
{
	float slashing;
	float blunt;
	float fallingDamage;
	float piercing;
	float fogDamage;
	Damage()
	{
		slashing = .0f;
		blunt	= .0f;
		piercing = .0f;
		fallingDamage = .0f;
		fogDamage = .0f;
	}

	float GetTotal() const
	{
		return slashing + blunt + piercing + fallingDamage + fogDamage;
	}

	float GetBleedFactor() const
	{
		// TODO: Balance
		return (piercing + slashing) / GetTotal();
	}
};