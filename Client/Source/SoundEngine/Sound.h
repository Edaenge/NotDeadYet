#pragma once
#include "AudioManager.h"
#include "EventHandle.h"
#include "sounds.h"
#include <Vector.h>


class Sound
{
private:
	float zCurrentTime;
	float zMaxTime;

	IEventHandle* zEvent;

public:
	Sound(int eventId);
	virtual ~Sound();

	void Update(float dt);
	bool IsPlaying();

	bool Play();
	bool Stop();

	void SetPosition(Vector3 vec);
};