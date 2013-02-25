#pragma once
#include "IEventHandle.h"

class EventHandle : public IEventHandle
{
	
	public:
		EventHandle(FMOD::Event* eventI);
		virtual int Play();
		virtual int Stop();
		virtual int TogglePause();
		virtual int SetParameter(int paramater, void* value);
		virtual int Release();
		virtual int Setposition(FMOD_VECTOR* position);
		virtual int Setvolume(float volume);
		virtual ~EventHandle();

	private:
		FMOD::Event* eventInstance;

};