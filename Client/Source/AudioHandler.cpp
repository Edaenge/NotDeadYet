#include "AudioHandler.h"

AudioHandler* AudioHandler::pnk = NULL;
bool AudioHandler::isInit = false;

#define NROFFOOTSTEPS 6

AudioHandler::AudioHandler()
{
	this->zAm = AudioManager::GetInstance();

	this->zBreathCounter = 0;
	this->zFootStepCounter = 0;

	for(int i = 0; i < NROFFOOTSTEPS; i++)
		zAm->GetEventHandle(EVENTID_NOTDEADYET_WALK_GRASS, this->zFootStepHandler[i]);
}
AudioHandler::~AudioHandler(){}


AudioHandler* AudioHandler::getInstance()
{
	if(!isInit)
	{
		pnk = new AudioHandler();
		isInit = true;
	}
	return pnk;
}

FMOD_VECTOR AudioHandler::ConvertToFmodVector( Vector3 v )
{
	FMOD_VECTOR temp;
	temp.x = v.x;
	temp.y = v.y;
	temp.z = v.z;
	return temp;
}

void AudioHandler::PlaySound( int eventId, Vector3 pos )
{
	FMOD_VECTOR temp = this->ConvertToFmodVector(pos);
	switch(eventId)
	{
		case EVENTID_NOTDEADYET_WALK_GRASS:
			this->zFootStepHandler[this->zFootStepCounter]->Setposition(&temp);
			this->zFootStepHandler[this->zFootStepCounter]->Play();
			this->zFootStepCounter++;
			if(this->zFootStepCounter >= NROFFOOTSTEPS)
				this->zFootStepCounter = 0;
			break;
		default:
			break;
	}

}

