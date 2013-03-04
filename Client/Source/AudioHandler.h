#pragma once

#include "sounds.h"
#include "AudioManager.h"
#include "vector.h"

class AudioHandler
{
private:
	AudioHandler();

	FMOD_VECTOR ConvertToFmodVector( Vector3 v );


	static AudioHandler* pnk;
	static bool isInit;
	AudioManager* zAm;

	int zBreathCounter;
	int zFootStepCounter;

	IEventHandle* zFootStepHandler[];
public:
	static AudioHandler* getInstance();
	virtual ~AudioHandler();
	virtual void PlaySound(int eventId, Vector3 pos);

};