#include "EventHandle.h"

EventHandle::EventHandle(FMOD::Event* eventI)
{

	this->eventInstance = eventI;
}

int EventHandle::Play()
{
	FMOD_EVENT_STATE state;
	eventInstance->getState(&state);

	if(state != FMOD_EVENT_STATE_READY)
		return eventInstance->start();
}

int EventHandle::Stop()
{

	return eventInstance->stop();

}

int EventHandle::TogglePause()
{

	bool paused;
	FMOD_RESULT result =  eventInstance->getPaused(&paused);

	if(result != FMOD_OK)
	{

		return result;

	}

	result = eventInstance->setPaused(!paused);
	return result;

}

int EventHandle::SetParameter(int paramater, void* value)
{

	return eventInstance->setPropertyByIndex(paramater,value,true);

}

int EventHandle::Release()
{

	return eventInstance->release(true,true);

}

int EventHandle::Setposition(FMOD_VECTOR* position)
{

	return eventInstance->set3DAttributes(position,0);

}

int EventHandle::Setvolume(float volume) 
{

	return eventInstance->setVolume(volume);
}

EventHandle::~EventHandle()
{

	eventInstance->release();

}
