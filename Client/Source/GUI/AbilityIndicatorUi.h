#pragma once

#include "Graphics.h"

enum INDICATORS{
	HEALTH = 0, // MUST BE ON 0
	HUNGER = 1, // MUST BE ON 1
	HYDRATION = 2, // MUST BE ON 2
	INDICATORCOUNTER
};

class AbilityIndicatorUi
{
private:
	iImage* zBG;
	iImage* zFillerBar;

	Vector3 zPos;
	Vector2 zDim;

	float zValue;
	float zMaxValue;
public:
	AbilityIndicatorUi(Vector3 pos, Vector2 dim, int type);
	virtual ~AbilityIndicatorUi();

	void Update(float value);
};