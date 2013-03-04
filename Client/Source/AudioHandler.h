#pragma once

#include "sounds.h"
#include "AudioManager.h"
#include "vector.h"

class AudioHandler
{
private:
	float zBreathCounter;
	float zFootStepCounter;

	IEventHandle* zFootStepHandler;
public:
	AudioHandler();
	virtual ~AudioHandler();
	void PlaySound(int eventId, Vector3 pos);

	FMOD_VECTOR ConvertToFmodVector( Vector3 v );
};