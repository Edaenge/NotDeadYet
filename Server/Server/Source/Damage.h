#pragma once

struct Damage
{
	float slashing;
	float blunt;
	float fallingDamage;
	float piercing;

	Damage()
	{
		slashing = .0f;
		blunt	= .0f;
		piercing = .0f;
		fallingDamage = .0f;
	}

	float GetTotal() const
	{
		return slashing + blunt + piercing + fallingDamage;
	}

	float GetBleedFactor() const
	{
		// TODO: Balance
		return (piercing + slashing) / GetTotal();
	}
};