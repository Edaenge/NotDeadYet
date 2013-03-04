#include "AudioHandler.h"

AudioHandler::AudioHandler()
{
	this->zBreathCounter = 0;
	this->zFootStepCounter = 0;
}
AudioHandler::~AudioHandler(){}

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

}
