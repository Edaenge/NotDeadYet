//Made by Berg Wilhelm 21/11/12 created) for project Not Dead Yet at Blekinge tekniska högskola


#pragma once
#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H
#include "fmod.hpp"
#include "fmod_errors.h"
#include "stdafx.h"

#define SOUNDS_NONE				0
#define MUSIC_NONE				0


class SoundHandler
{
private:
	FMOD::System*		zSystem;
	FMOD::Geometry*		zGeometry;
	FMOD::Sound*		zSounds[16];	//This should probably be changed
	FMOD::Sound*		zTrack1;		//Just an example for now
	FMOD::Channel*		zSoundChannel; 
	FMOD::Channel*		zMusicChannel;
	FMOD::ChannelGroup*	zSoundGroup;
	FMOD::ChannelGroup*	zMusicGroup;
	FMOD::ChannelGroup*	zMasterGroup;
	FMOD::DSP*			zEcho;		//Echo, probably the most important applied effect
	FMOD::DSP*			zLowpass;
	FMOD::DSP*			zParameq; 

	FMOD_VECTOR			zSoundPos;
	FMOD_VECTOR			zPlayerPos;
	FMOD_VECTOR			zForwardVector;
	FMOD_VECTOR			zUpVector;

	int					zNrOfSounds;
	unsigned int		zVersion;
	bool				zIsPaused;
	bool				zIsMuted;

public:
				SoundHandler();
	virtual		~SoundHandler();

	int			Init();
	void		PlaySound(int sound, D3DXVECTOR3 p_sPos); //The sound integer is to see which sound should be played.
	void		PauseAllAudio();
	void		PlayMusic(int music); //The music integer is to see which music should be played.
	void		StopMusic();	//Music loops, so we must be able to stop it.

	void		SetSoundVolume(float volume);
	void		SetMusicVolume(float volume);
	void		Mute();

	//void		SetEffect(int effectNumber);//adds and removes effects, does not currently work.

	void		updateSystem();
	void		UpdateListenerPos(D3DXVECTOR3 p_pPos, D3DXVECTOR3 forward, D3DXVECTOR3 up); //There is more things you can do here.

	void		ERRCHECK(FMOD_RESULT result);
};
#endif