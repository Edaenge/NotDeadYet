/*!
	Simple class for playing sounds and songs in 2D.
*/


#pragma once

#include "SoundEffect.h"
#include "SoundSong.h"
#include <vector>


class SoundEngine
{
	private:
		// System
		FMOD::System*		mSystem;
		FMOD_RESULT			mResult;		//default value: FMOD_ERR_NOTREADY.
		unsigned int		mVersion;
		int					mNrOfDrivers;
		FMOD_SPEAKERMODE	mSpeakerMode;	//default value: FMOD_SPEAKERMODE_STEREO.
		FMOD_CAPS			mCaps;			//default value: FMOD_CAPS_NONE.
		char*				mName;

		// All sound
		float*				mMasterVolume;	//default value: 1.0f

		// Sound effects 2D
		std::vector<SoundEffect*> mEffects2D;

		// Sound effects 3D todo**
		/*
		unsigned int		mNrOfSoundFX3D;
		unsigned int		mSoundFXCap3D;
		FMOD::Sound**		mSoundFX3D;
		unsigned int		mSoundFXChannelsCap3D;
		FMOD::Channel**		mSoundFXChannels3D;
		float				mDistanceFactor;//default value: 100.0f. Units per meter (100 cm).
		*/

		// Songs (2D)
		std::vector<SoundSong*> mSongs;

	private:
		void ERRCHECK(FMOD_RESULT result);

	public:
		SoundEngine();
		virtual ~SoundEngine();

		int Init();

		//Set functions
		/*! Set the master volume. Range is [0,1]. */
		void SetMasterVolume(float volume);
		/*! Set the sound effect volume. Range is [0,1]. */
		void SetSoundEffectVolume(float volume);
		/*! Set the song volume. Range is [0,1]. */
		void SetSongVolume(float volume);
		/*! Set debug level. */
		void SetDebugLevel(unsigned int level);

		//Sound effects
		/*! Load sound effect from soundfile. **3D currently unavailable** */
		SoundEffect* LoadSoundEffect(std::string filename, bool as3D = false);

		
		//Song
		/*! Load song from soundfile. */
		SoundSong* LoadSong(std::string filename, bool loop);


		//**todo**
		//void Play3DSoundEffect(unsigned int soundEffectIndex, D3DXVECTOR3 position);
		
};