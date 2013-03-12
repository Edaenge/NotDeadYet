#pragma once

#include <windows.h>
#include <vector>

class GameTimer
{
public:
	GameTimer();
	virtual ~GameTimer() {}
	void Init();
	float Frame();
	void CalculateFps(float deltaTime);
	inline float GetRunTime() {return this->zRunTime;}
	inline float GetDeltaTime() {return this->zDeltaTime;}
	inline int GetFPS() {return this->zFramesPerSec;}

private:
	float zSecsPerCnt;
	INT64 zStartTime;
	float zDeltaTime;
	float zRunTime;
	int zFramesPerSec;
	int zFrameCounter;
	float zCounter;
	std::vector<float> zDt;
};