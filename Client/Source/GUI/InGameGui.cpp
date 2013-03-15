#include "InGameGui.h"
#include "AbilityIndicatorUi.h"



InGameGui::InGameGui()
{
	float windowWidth = (float)GetGraphics()->GetEngineParameters().WindowWidth;
	float windowHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	Vector2 imageDim = Vector2((30.0f / 1024.0f) * dx, (150.0f / 768.0f) * windowHeight);

	Vector3 healthPos = Vector3(offSet + (HEALTHXPOS / 1024.0f) * dx, (ALLYPOS / 768.0f) * windowHeight, 100.0f);

	Vector3 hungerPos = Vector3(offSet + (HUNGERXPOS / 1024.0f) * dx, (ALLYPOS / 768.0f) * windowHeight, 100.0f);

	Vector3 hydratonPos = Vector3(offSet + (HYDRATIONXPOS / 1024.0f) * dx, (ALLYPOS / 768.0f) * windowHeight, 100.0f);

	this->zAiu[HEALTH] = new AbilityIndicatorUi(healthPos, imageDim, HEALTH);
	this->zAiu[HUNGER] = new AbilityIndicatorUi(hungerPos, imageDim, HUNGER);
	this->zAiu[HYDRATION] = new AbilityIndicatorUi(hydratonPos, imageDim, HYDRATION);
}

InGameGui::~InGameGui()
{
	for(int i = 0; i < INDICATORCOUNTER; i++)
	{
		delete this->zAiu[i];
	}
}

void InGameGui::Update( float health, float hunger, float hydration )
{
	this->zAiu[HEALTH]->Update(health, HEALTHXPOS, ALLYPOS);
	this->zAiu[HUNGER]->Update(hunger, HUNGERXPOS, ALLYPOS);
	this->zAiu[HYDRATION]->Update(hydration, HYDRATIONXPOS, ALLYPOS);
}
