#ifndef _I_AUDIO_EVENTS_HPP_
#define _I_AUDIO_EVENTS_HPP_

#pragma once

#include "IEventHandle.h"

///<summary>The interface to handle events as well as the players position</summary>
class IAudioEvents
{

	public:
		///<Summary>Gets an new instance of an eventhandle, the eventdata has to have been loaded first or it will crash and burn</summary>
		///<param name="group">The EVENTGROUP-index for the group to load, see sounds.h</param>
		///<param name="eventId">The EVENTGROUP-relative index for the event to load, see sounds.h</param>
		///<param name="handle">The pointer for the handle to insert the eventhandle in</param>
		///<returns>A value indicating success, anything but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int GetEventHandle(int group,int eventId,IEventHandle* &handle) = 0;
		
		///<Summary>Informs the system of the players current position, the system must have been initialized first</summary>
		///<param name="position">The position vector in worldspace</param>
		///<param name="forward">The forward vector, this must be perpendicular with the up-vector</param>
		///<param name="up">The up vector, this must be perpendicular with the forward-vector</param>
		///<returns>A value indicating success, anything but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int SetPlayerPosition(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up) = 0;
		
		///<Summary>Gets an new instance of an eventhandle, the FEV-file has to have been loaded first but the eventdata doesn't and will be loaded on call, this may result in a certain slowdown so use wisely</summary>
		///<param name="group">The Project-unique event id for the group to load, see sounds.h</param>
		///<param name="handle">The pointer for the handle to insert the eventhandle in</param>
		///<returns>A value indicating success, anything but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int GetEventHandle(int eventId,IEventHandle* &handle) = 0;
};

#endif // _I_AUDIO_EVENTS_HPP_