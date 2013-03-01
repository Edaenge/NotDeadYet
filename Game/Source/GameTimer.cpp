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
	
	//Calculate Frames Per Second
	this->zCounter += this->zDeltaTime;

	this->zFrameCounter++;
	if(this->zCounter >= 1.0f)
	{
		this->zFramesPerSec = this->zFrameCounter;

		this->zCounter = 0.0f;
		this->zFrameCounter = 0;
	}

	return this->zDeltaTime;
}
