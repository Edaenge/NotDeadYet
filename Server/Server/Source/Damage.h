#pragma once

struct Damage
{
	float slashing;
	float blunt;
	
	float GetTotal() const
	{
		return slashing + blunt;
	}

	float GetBleedFactor() const
	{
		// TODO: Balance
		return slashing / GetTotal();
	}
};