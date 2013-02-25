#ifndef _I_AUDIO_MANAGER_HPP_
#define _I_AUDIO_MANAGER_HPP_

#pragma once

class IAudioManager
{
	public:
		///<Summary>Updates the underlying FMOD-engine, this method must be called at least once a frame</summary>
		virtual void Update() = 0;
		
		///<Summary>Sets the volume of a catagory and all events under it</summary>
		///<param name="index">The index for the catagory to change, indices are 0-based</param>
		///<param name="value">The volume to change to, this ranges between 0.0 and 1.0</param>
		///<returns>A value indicating success, anything but the value 0 is some form of failure, see FMOD_RESULT in fmod.h for more detail</returns>
		virtual int SetVolume(int index,float value) = 0;
};

#endif // _I_AUDIO_MANAGER_HPP_