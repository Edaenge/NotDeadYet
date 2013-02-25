/*
	Made by Jensen Christopher Datum(17/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "Actor.h"
#include <vector>
#include <map>
#include "AudioManager.h"

class Updates
{
public:
	Updates()
	{
		this->zID = 0;
		this->zPositionChange = false;
		//this->zRotationChange = false;
	}
	Updates(const unsigned int ID) 
	{
		this->zID = ID;
		//this->zStateChange = false;
		this->zPositionChange = false;
		//this->zRotationChange = false;
	}

	unsigned int GetID() const {return this->zID;}
	Vector3 GetPosition() const {return this->zNextPosition;}
	//Vector4 GetRotation() const {return this->zRotation;}
	//unsigned int GetState() const {return this->zState;}

	bool HasPositionChanged() const {return this->zPositionChange;}
	//bool HasRotationChanged() const {return this->zRotationChange;}
	//bool HasStateChanged() const {return this->zStateChange;}

	void SetPosition(Vector3 position) 
	{
		this->zNextPosition = position;
		this->zPositionChange = true;
	}
	//void SetRotation(Vector4 rotation) 
	//{
	//	this->zRotation = rotation;
	//	this->zRotationChange = true;
	//}
	//void SetState(unsigned int state) 
	//{
	//	this->zState = state;
	//	this->zStateChange = true;
	//}
	bool ComparePosition(const Vector3& position)
	{
		if ((this->zNextPosition - position).GetLength() < 0.5f)
			this->zPositionChange = false;

		return this->zPositionChange;
	}
	//bool CompareRotation(const Vector4& rotation)
	//{
	//	if ((this->zRotation - rotation).GetLength() < 0.5f)
	//		this->zRotationChange = false;

	//	return this->zRotationChange;
	//}
	//void SetStateChange(bool value)
	//{
	//	this->zStateChange = value;
	//}
	//void SetRotationChanged(bool value)
	//{
	//	this->zRotationChange = value;
	//}
private:
	unsigned int zID;
	Vector3 zNextPosition;
	//Vector4 zRotation;
	//unsigned int zState;

	//bool zStateChange;
	bool zPositionChange;
	//bool zRotationChange;
};

static const enum INTERPOLATION_TYPES
{
	IT_LINEAR,
	IT_COSINE,
	IT_ACCELERATION,
	IT_SMOOTH_STEP,
	IT_DECELERATION
};

class ClientActorManager
{
public:
	ClientActorManager();
	virtual ~ClientActorManager();

	/*! Interpolates all the Objects towards their final Position*/
	void UpdateObjects(float deltaTime, unsigned int ignoreID);

	bool AddActor(Actor* actor);
	Actor* GetActor(unsigned int ID);
	std::map<unsigned int, Actor*> GetActors();
	void RemoveActor(const unsigned int ID);

	void AddUpdate(Updates* update);
	Updates* GetUpdate(const int ID);

	Vector4 InterpolateRotation(const Vector4& currentRotation, const Vector4& newRotation, float t);
	Vector3 InterpolatePosition(const Vector3& currentPosition, const Vector3& newPosition, float t);
	/*! Returns time Value depending on type
	IT_LINEAR,
	IT_COSINE,
	IT_ACCELERATION,
	IT_SMOOTH_STEP,
	IT_DECELERATION
	*/
	float GetInterpolationType(const float deltaTime, const unsigned int type);
	void SetCameraOffset(Vector3 offset) {this->zCameraOffset = offset;}
	Vector3 GetCameraOffset() {return this->zCameraOffset;}

private:
	std::map<unsigned int, Actor*> zActors;
	std::map<unsigned int, Actor*> zState;
	std::map<unsigned int, Updates*> zUpdates;
	float zInterpolationVelocity;
	Vector3 zCameraOffset;

	FMOD_VECTOR ConvertToFmodVector(Vector3 v);
	IEventHandle* footStep;
};