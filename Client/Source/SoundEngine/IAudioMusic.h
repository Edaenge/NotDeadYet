#ifndef _I_AUDIO_MUSIC_HPP_
#define _I_AUDIO_MUSIC_HPP_

#pragma once

///<Summary>The interface to handle all music-playing</summary>
class IAudioMusic
{
	public:
		///<Summary>Starts playing a music-cue, the FEV-file must have been loaded first</summary>
		///<param name="cue">The Music cue id to play, see sounds.h</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int PlayMusicCue(int cue) = 0;
		
		///<Summary>Stops playing the current music-cue, the FEV-file must have been loaded first and a cue must be playing</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int StopMusicCue() = 0;
		
		///<Summary>Pauses the current music-cue, the FEV-file must have been loaded first and a cue must be playing</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int TogglePauseMusicCue() = 0;
		
		///<Summary>Sets a parameter used by music-cues, the FEV-file must have been loaded first</summary>
		///<param name="parameter">The Music parameter id to change, see sounds.h</param>
		///<param name="value">The value the parameter should have</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int SetMusicParameters(int parameter,float value) = 0;
};

#endif // _I_AUDIO_MUSIC_HPP_