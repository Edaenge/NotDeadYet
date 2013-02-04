#include "Sounds.h"

namespace {
	static SoundHandler* sound_engine = 0;
}

SoundHandler* GetSounds()
{
	if ( !sound_engine )
		throw("Engine Not Initialized!");

	return sound_engine;
}

bool SoundsInit()
{
	sound_engine = new SoundHandler();

	bool result =  sound_engine->Init();

	return result;
}

bool FreeSounds()
{
	if (!sound_engine)
		return false;
	
	delete sound_engine;

	return true;
}