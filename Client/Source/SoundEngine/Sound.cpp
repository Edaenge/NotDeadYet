#include "Sound.h"

Sound::Sound(int eventId)
{
	AudioManager* am = AudioManager::GetInstance();
	am->GetEventHandle(eventId, this->zEvent);
	this->zCurrentTime = 0;

	switch (eventId)
	{
	case EVENTID_NOTDEADYET_WALK_GRASS:
		this->zMaxTime = 0.5f;
		break;
	case EVENTID_NOTDEADYET_WALK_DIRT:
		this->zMaxTime = 0.5f;
		break;
	default:
		break;
	}
}

Sound::~Sound()
{
	this->zEvent->Release();
	delete this->zEvent;
}

void Sound::Update( float dt )
{
	if(this->zCurrentTime >= this->zMaxTime)
		return;

	this->zCurrentTime += dt;
}

bool Sound::IsPlaying()
{
	if(this->zCurrentTime < this->zMaxTime)
		return true;

	return false;
}

bool Sound::Play()
{
	if(this->zCurrentTime < this->zMaxTime)
		return false; // Return false if already playing.

	if(this->zEvent->Play() == FMOD_OK) // If sound is played with no problems return true.
	{
		this->zCurrentTime = 0.0f;
		return true;
	}
	
	return false; // Else return false.
}
bool Sound::Stop()
{
	if(this->zEvent->Stop() == FMOD_OK)
		return true;

	return false;
}

void Sound::SetPosition( Vector3 vec )
{
	FMOD_VECTOR temp;
	temp.x = vec.x;
	temp.y = vec.y;
	temp.z = vec.z;
	this->zEvent->Setposition(&temp);
}

