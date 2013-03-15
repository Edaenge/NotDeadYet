#pragma once
class AbilityIndicatorUi;

#define NROFINDICATORS 3

#define ALLYPOS 610.0f
#define HEALTHXPOS 870.0f
#define HUNGERXPOS 920.0f
#define HYDRATIONXPOS 970.0f 

class InGameGui
{
private:
	AbilityIndicatorUi* zAiu[NROFINDICATORS];
public:
	InGameGui();
	virtual ~InGameGui();

	void Update(float health, float hunger, float hydration);
};