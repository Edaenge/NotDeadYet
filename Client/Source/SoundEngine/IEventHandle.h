#ifndef _I_EVENT_HANDLE_HPP_
#define _I_EVENT_HANDLE_HPP_

#pragma once

#include "Fmod.hpp"

///<summart>The eventhandle to play sounds and events</summary>
class IEventHandle
{
	public:
		///<Summary>Plays this event, if it's already playing nothing happens</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int Play() = 0;
		
		///<Summary>Stop this event, only really necessary on looping events, others will stop on their own</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int Stop() = 0;
		
		///<Summary>Toggles the pause for this event</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int TogglePause() = 0;
		
		///<Summary>Sets a parameter for this event</summary>
		///<param name="paramater">The paramater id, see sounds.h</param>
		///<param name="value">The value it should have</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int SetParameter(int paramater, void* value) = 0;

		///<Summary>Releases this instance and puts the resources back in the pool, call this when done with event</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int Release() = 0;

		///<Summary>Sets the events position in world-space</summary>
		///<param name="position">The 3d-position this sound should have in world-space, if event isn't in 3d-mode this does nothing</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int Setposition(FMOD_VECTOR* position) = 0;

		///<Summary>Sets the volume for this instance of an event, do not use this to simulate distance!, it won't work well<summary>
		///<param name="volume">The volume to set the index to, this varies between 0.0 to 1.0</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int Setvolume(float volume) = 0;
};

#endif // _I_EVENT_HANDLE_HPP_