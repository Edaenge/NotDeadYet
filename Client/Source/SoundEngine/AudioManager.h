#ifndef _AUDIO_MANAGER_HPP_
#define _AUDIO_MANAGER_HPP_

#pragma once

#include <string>

class AudioManager;

#include "Fmod.hpp"
#include "IAudioManager.h"
#include "IAudioLoader.h"
#include "IAudioMusic.h"
#include "IAudioEvents.h"
#include "sounds.h"

class AudioManager: public IAudioManager, public IAudioLoader, public IAudioMusic, public IAudioEvents
{
	private:
		AudioManager();
		int init();
		int release();

		static AudioManager* pnk;
		static bool isInited;

		int groups;
		int cats;
		FMOD::EventSystem* system;
		FMOD::MusicSystem* mSystem;
		FMOD::MusicPrompt* currentCue;
		FMOD::EventProject* projectFile;
		FMOD::EventGroup* root;
		FMOD::EventGroup** eventGroups;
		FMOD::EventCategory** categories;

		virtual ~AudioManager();

	public:
		///<Summary>Gets and initializes the instance, if instance already is initialized it just gets it</summary>
		///<Returns>The instance</returns>
		static AudioManager* GetInstance();
		///<Summary>Releases the instance along with any and ALL resources associated with it</summary>
		static void ReleaseInstance();

		//AudioManager
		virtual void Update();
		virtual int SetVolume(int index,float value);

		//AudioEvents
		virtual int GetEventHandle(int group,int eventId,IEventHandle* &handle);
		virtual int SetPlayerPosition(FMOD_VECTOR* position,FMOD_VECTOR* forward,FMOD_VECTOR* up);
		virtual int GetEventHandle(int eventId,IEventHandle* &handle);

		//AudioMusic
		virtual int PlayMusicCue(int cue);
		virtual int StopMusicCue();
		virtual int TogglePauseMusicCue();
		virtual int SetMusicParameters(int parameter,float value);

		//AudioLoader
		virtual int LoadEventData(int eventGroup);
		virtual int UnloadEventData(int eventGroup);
		virtual int LoadFEV(std::string file);
		virtual int UnloadFEV();

};

#endif // _AUDIO_MANAGER_HPP_