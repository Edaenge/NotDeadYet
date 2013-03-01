#pragma once

#include <windows.h>

class GameTimer
{
private:
	float zSecsPerCnt;
	INT64 zStartTime;
	float zDeltaTime;
	float zRunTime;
	int zFramesPerSec;
	int zFrameCounter;
	float zCounter;
public:
	GameTimer();
	void Init();
	float Frame();
	inline float GetRunTime() {return this->zRunTime;}
	inline float GetDeltaTime() {return this->zDeltaTime;}
	inline int GetFPS() {return this->zFramesPerSec;}
};