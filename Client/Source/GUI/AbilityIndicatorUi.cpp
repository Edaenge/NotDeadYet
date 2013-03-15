#include "AbilityIndicatorUi.h"
#include "Graphics.h"
#include "PlayerConfigReader.h"

#define BARPATHHEALTH "Media/InGameUI/Hp_hud_bar.png"
#define BGPATHHEALTH "Media/InGameUI/Hp_hud.png"

#define BARPATHHUNGER "Media/InGameUI/Hp_hud_bar.png"
#define BGPATHHUNGER "Media/InGameUI/Hp_hud.png"

#define BARPATHHYDRATION "Media/InGameUI/Hp_hud_bar.png"
#define BGPATHHYDRATION "Media/InGameUI/Hp_hud.png"

AbilityIndicatorUi::AbilityIndicatorUi(Vector3 pos, Vector2 dim, int type)
{
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

void AbilityIndicatorUi::Update( float value )
{
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

