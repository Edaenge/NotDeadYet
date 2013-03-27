#include "../Include/GameTimer.h"

GameTimer::GameTimer()
{
	this->zCounter = 0.0f;
	this->zRunTime = 0.0f;
	this->zDeltaTime = 0.0f;
	this->zSecsPerCnt = 0.0f;

	this->zFramesPerSec = 0;
	this->zFrameCounter = 0;
}

void GameTimer::Init()
{
	INT64 frequency;

	QueryPerformanceFrequency( (LARGE_INTEGER*) &frequency);

	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter( (LARGE_INTEGER*) &this->zStartTime);
}

float GameTimer::Frame()
{
	//Calculate DeltaTime
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter( (LARGE_INTEGER*) &currentTime);

	timeDifference = (float) (currentTime - this->zStartTime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zRunTime += this->zDeltaTime;

	this->zStartTime = currentTime;

	return this->zDeltaTime;
}

void GameTimer::CalculateFps(float deltaTime)
{
	static const unsigned int MAX_SAMPLES = 50;

	if (this->zDt.size() >= MAX_SAMPLES)
		this->zDt.erase(this->zDt.begin());

	this->zDt.push_back(deltaTime);

	float totalDeltaTime = 0.0f;
	auto it_end = this->zDt.end();
	for (auto it = this->zDt.begin(); it != it_end; it++)
	{
		totalDeltaTime += (*it);
	}

	float averageDelta = totalDeltaTime / this->zDt.size();

	this->zFramesPerSec = (int)(1.0f / averageDelta);
}