#include "InGameGui.h"
#include "AbilityIndicatorUi.h"

enum INDICATORS{
	HEALTH,
	HUNGER,
	HYDRATION,
	INDICATORCOUNTER
};

InGameGui::InGameGui()
{
	this->zAiu[HEALTH] = new AbilityIndicatorUi();
	this->zAiu[HUNGER] = new AbilityIndicatorUi();
	this->zAiu[HYDRATION] = new AbilityIndicatorUi();
}

InGameGui::~InGameGui()
{

}
