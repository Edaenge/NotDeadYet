/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "Actor.h"
#include <vector>
#include <map>
#include "AudioManager.h"
#include "AnimationFileReader.h"
#include "World/World.h"
#include "Sound.h"


class Updates
{
	unsigned int zID;
	Vector3 zNextPosition;
	unsigned int zState;
	bool zStateChange;
	bool zPositionChange;

public:
	Updates(const unsigned int& ID=0) 
	{
		this->zID = ID;
		this->zPositionChange = false;
	}

	inline const unsigned int& GetID() const { return this->zID; }
	inline const Vector3& GetPosition() const { return this->zNextPosition; }
	inline const unsigned int& GetState() const { return this->zState; }
	inline const bool& HasStateChanged() const { return this->zStateChange; }
	inline const bool& HasPositionChanged() const { return this->zPositionChange; }

	void SetPosition(const Vector3& position) 
	{
		this->zNextPosition = position;
		this->zPositionChange = true;
	}

	bool ComparePosition(const Vector3& position)
	{
		if ((this->zNextPosition - position).GetLength() < 0.5f)
		{
			this->zPositionChange = false;
		}

		return this->zPositionChange;
	}

	void SetState(const unsigned int& state) 
	{
		this->zState = state;
		this->zStateChange = true;
	}

	void SetStateChange(const bool& value)
	{
		this->zStateChange = value;
	}
};

static const enum INTERPOLATION_TYPES
{
	IT_LINEAR,
	IT_COSINE,
	IT_ACCELERATION,
	IT_SMOOTH_STEP,
	IT_DECELERATION
};

class ActorAddedEvent : public Event
{
public:
	ClientActorManager* zActorManager;
	Actor* zActor;
};

class ActorRemovedEvent : public Event
{
public:
	ClientActorManager* zActorManager;
	Actor* zActor;
};

class ClientActorManager : public Observed
{	
	float zInterpolationVelocity;
	int zUpdatesPerSec;
	int zLatency;
	Vector3 zCameraOffset;

	Sound** zFootStepsOnGrass;
	Sound** zFootStepsOnDirt;

	std::map<unsigned int, Actor*> zActors;
	std::map<Actor*, unsigned int> zState;
	std::map<unsigned int, Updates*> zUpdates;
	std::map<std::string, AnimationFileReader> zModelToReaderMap;

public:
	ClientActorManager();
	virtual ~ClientActorManager();

	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(const float& deltaTime, const unsigned int& ignoreID, World* world);

	unsigned int GetState(Actor* actor);
	void AddActorState(Actor* actor, const unsigned int& state);
	bool AddActor(Actor* actor);
	Actor* GetActor(const unsigned int& ID);
	void RemoveActor(const unsigned int& ID);

	void AddUpdate(Updates* update);
	Updates* GetUpdate(const unsigned int& ID) const;

	Vector4 InterpolateRotation(const Vector4& currentRotation, const Vector4& newRotation, float t);
	Vector3 InterpolatePosition(const Vector3& currentPosition, const Vector3& newPosition, float t);

	inline const std::map<unsigned int, Actor*>& GetActors() const { return this->zActors; }
	inline void SetFBXMapping(const std::map<std::string, AnimationFileReader>& map) { this->zModelToReaderMap = map; }
	inline void SetLatency(const int& latency) { this->zLatency = latency; }
	inline void SetUpdatesPerSec(const int& ups) { this->zUpdatesPerSec = ups; }
	inline void SetCameraOffset(const Vector3& offset) { this->zCameraOffset = offset; }
	inline const Vector3& GetCameraOffset() { return this->zCameraOffset; }

	/*! Returns time Value depending on type
	IT_LINEAR,
	IT_COSINE,
	IT_ACCELERATION,
	IT_SMOOTH_STEP,
	IT_DECELERATION
	*/
	float GetInterpolationType(const float& deltaTime, const unsigned int& type);
	
	/*! Deletes all actors and updates.*/
	void ClearAll();

protected:
	FMOD_VECTOR ConvertToFmodVector(const Vector3& v) const;
	unsigned int GetMostUsedTexOnPos(const Vector2& pos, World* world) const;
};