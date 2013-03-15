#pragma once
class AbilityIndicatorUi;

#define NROFINDICATORS 3

class InGameGui
{
private:
	AbilityIndicatorUi* zAiu[NROFINDICATORS];
public:
	InGameGui();
	virtual ~InGameGui();

	void Update(float health, float hunger, float hydration);
};