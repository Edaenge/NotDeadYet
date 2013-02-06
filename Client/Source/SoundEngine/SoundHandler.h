//Made by Berg Wilhelm 21/11/12 created) for project Not Dead Yet at Blekinge tekniska högskola


#pragma once
#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include "fmod.hpp"
#include "fmod_errors.h"
#include "Vector.h"
#include <fstream>
#include <string>
#include <list>

//struct APolygon
//{
//	int numVertices;
//	int indicesOffset;
//	float directOcclusion;
//	float reverbOcclusion;
//	FMOD_VECTOR normal;
//};
//
//struct ObjectMesh
//{
//	int numVertices;
//	FMOD_VECTOR *vertices;
//	int numPolygons;
//	APolygon* polygons;
//	int numIndices;
//	int *indices;
//	FMOD::Geometry *geometry;
//};

struct SoundStruct
{
	FMOD::Sound*	sound;
	char*			name;

	SoundStruct(FMOD::Sound* sound, char* name) { this->sound = sound; this->name = name; };
};


class SoundHandler
{
private:
	FMOD::System*		zSystem;
	std::list<SoundStruct> zSoundList;
	std::list<SoundStruct> zMusicList; 
	FMOD::Channel*		zSoundChannel; 
	FMOD::Channel*		zMusicChannel;
	FMOD::ChannelGroup*	zSoundGroup;
	FMOD::ChannelGroup*	zMusicGroup;
	FMOD::ChannelGroup*	zMasterGroup;
	//FMOD::DSP*			zEcho;		//Echo, probably the most important applied effect
	//FMOD::DSP*			zLowpass;
	//FMOD::DSP*			zParameq;
	
	

	FMOD::Geometry*		zLandGeometry;
	FMOD::Geometry**	zObjectGeometry;

	//ObjectMesh*			zLandscape; //The mesh to use for the landscape sound blocking.
	//ObjectMesh*			zObjects;	//The meshes to use for blocking sounds, trees, rocks, etc.

	FMOD_VECTOR			zSoundPos;
	FMOD_VECTOR			zPlayerPos;
	FMOD_VECTOR			zForwardVector;
	FMOD_VECTOR			zUpVector;

	int					zNrOfGeometryObjects;
	unsigned int		zVersion;
	bool				zIsPaused;
	bool				zIsMuted;

public:
				SoundHandler();
	virtual		~SoundHandler();

	int			Init();
	
	bool		LoadSoundIntoSystem(const char* filename, bool loop) throw (...);
	bool		LoadMusicIntoSystem(const char* filename, bool loop) throw (...);
	//These three have no priority.
	//void		InitLandGeometry(/*Something that allows to get all vertices the land has, like a file name to be read*/); 
	//void		InitObjectGeometry(/*Something that allows knowing what object it is and its vertices information, and maybe rotation and pos too*/);
	//void		clearGeometry();
	void		PlaySounds(char* filename, Vector3 p_sPos); //The sound integer is to see which sound should be played.
	void		PauseAllAudio();
	void		PlayMusic(char* filename); //The music integer is to see which music should be played.
	void		StopMusic();	//Music loops, so we must be able to stop it.

	void		SetSoundVolume(float volume);
	void		SetMusicVolume(float volume);
	void		SetMasterVolume(float volume);
	void		Mute();

	//void		SetEffect(int effectNumber);//adds and removes effects, does not currently work.


	void		updateSystem();
	void		UpdateListenerPos(Vector3 p_pPos, Vector3 forward, Vector3 up); //There is more things you can do here.

	void		ERRCHECK(FMOD_RESULT result);


};
#endif