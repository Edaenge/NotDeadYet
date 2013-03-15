#include "AbilityIndicatorUi.h"
#include "Graphics.h"
#include "PlayerConfigReader.h"

#define BARPATHHEALTH "Media/InGameUI/Hp_hud_bar.png"
#define BGPATHHEALTH "Media/InGameUI/Hp_hud.png"

#define BARPATHHUNGER "Media/InGameUI/Hunger_hud_bar.png"
#define BGPATHHUNGER "Media/InGameUI/Hunger_hud.png"

#define BARPATHHYDRATION "Media/InGameUI/Thirst_hud_bar.png"
#define BGPATHHYDRATION "Media/InGameUI/Thirst_hud.png"

AbilityIndicatorUi::AbilityIndicatorUi(Vector3 pos, Vector2 dim, int type)
{
	this->zFitForX = (float)GetGraphics()->GetEngineParameters().WindowWidth;
	this->zFitForY = (float)GetGraphics()->GetEngineParameters().WindowHeight;

	this->zBG = NULL;
	this->zFillerBar = NULL;

	this->zPos = pos;
	this->zDim = dim;

	InitPlayerConfig();
	PlayerConfigReader* pcr = GetPlayerConfig();
	std::string bg = "";
	std::string filler = "";
	switch (type)
	{
	case HEALTH:
		this->zMaxValue = 100.0f;
		bg = BGPATHHEALTH;
		filler = BARPATHHEALTH;
		break;
	case HUNGER:
		this->zMaxValue = pcr->GetVariable(FULLNESS_MAX);
		bg = BGPATHHUNGER;
		filler = BARPATHHUNGER;
		break;
	case HYDRATION:
		this->zMaxValue = pcr->GetVariable(HYDRATION_MAX);
		bg = BGPATHHYDRATION;
		filler = BARPATHHYDRATION;
		break;
	default:
		this->zMaxValue = 0.0f;
		break;
	}
	this->zValue = 0.0f;

	Vector3 tempPos = this->zPos;
	Vector2 tempDim = this->zDim;
	tempDim.y = (this->zValue / this->zMaxValue) * this->zDim.y;
	tempPos.y += this->zDim.y - tempDim.y;

	this->zBG = GetGraphics()->CreateImage(this->zPos.GetXY(), this->zDim, bg.c_str());
	this->zFillerBar = GetGraphics()->CreateImage(tempPos.GetXY(), tempDim, filler.c_str());
	this->zBG->SetStrata(this->zPos.z - 10.0f);
	this->zFillerBar->SetStrata(this->zPos.z);
}

AbilityIndicatorUi::~AbilityIndicatorUi()
{
	FreePlayerConfig();
	if(this->zBG)
	{
		GetGraphics()->DeleteImage(this->zBG);
		this->zBG = NULL;
	}
	if(this->zFillerBar)
	{
		GetGraphics()->DeleteImage(this->zFillerBar);
		this->zFillerBar = NULL;
	}
		
}

void AbilityIndicatorUi::Update( float value, float x, float y )
{
	float windowWidth = (float)GetGraphics()->GetEngineParameters().WindowWidth;
	float windowHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;
	if(this->zFitForX != windowWidth && this->zFitForY != windowHeight)
	{
		this->zFitForX = windowWidth;
		this->zFitForY = windowHeight;

		float dx = (windowHeight * 4.0f) / 3.0f;
		float offSet = (windowWidth - dx) / 2.0f;

		this->zPos = Vector3(offSet + (x / 1024.0f) * dx, (y / 768.0f) * windowHeight, 100.0f);
		this->zDim = Vector2((30.0f / 1024.0f) * dx, (150.0f / 768.0f) * windowHeight);
	}

	this->zValue = value;

	Vector3 tempPos = this->zPos;
	Vector2 tempDim = this->zDim;
	tempDim.y = (this->zValue / this->zMaxValue) * this->zDim.y;
	tempPos.y += this->zDim.y - tempDim.y;

	this->zBG->SetDimensions(this->zDim);
	this->zBG->SetPosition(this->zPos.GetXY());

	this->zFillerBar->SetDimensions(tempDim);
	this->zFillerBar->SetPosition(tempPos.GetXY());
}

