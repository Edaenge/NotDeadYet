#ifndef _I_AUDIO_LOADER_HPP_
#define _I_AUDIO_LOADER_HPP_

#pragma once

#include <string>

///<Summary>The interface used for loading sounds</summary>
class IAudioLoader
{
	public:
		///<Summary>Loads all eventdata within a group</summary>
		///<param name="eventGroup">The EVENTGROUP-index for the group to load, see sounds.h</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int LoadEventData(int eventGroup) = 0;
		
		///<Summary>unloads all eventdata within a group, use this only after releasing related handles, this can naturellemont only be done after loading first</summary>
		///<param name="eventGroup">The EVENTGROUP-index for the group to unload, see sounds.h</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int UnloadEventData(int eventGroup) = 0;
		
		///<Summary>Loads a FEV-file containing information about events, this does not allocate memory to the events, use LoadEventData for that</summary>
		///<param name="file">The full path to the FEV-file</param>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int LoadFEV(std::string file) = 0;

		///<Summary>Unloads the currently loaded FEV-file along with all related data, this can naturellemont only be done after loading first</summary>
		///<returns>A value indicating success, anythin but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int UnloadFEV() = 0;
};

#endif // _I_AUDIO_LOADER_HPP_