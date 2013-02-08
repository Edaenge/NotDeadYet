#pragma once

struct Damage
{
	float slashing;
	float blunt;
	float fallingDamage;

	float GetTotal() const
	{
		return slashing + blunt + fallingDamage;
	}

	float GetBleedFactor() const
	{
		// TODO: Balance
		return slashing / GetTotal();
	}
};