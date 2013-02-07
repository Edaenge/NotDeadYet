/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include <string>
#include <Vector.h>
#include <Safe.h>
#include <Observer.h>
#include "PhysicsObject.h"

static const enum ACTOR_TYPE
{
	ACTOR_TYPE_NONE,
	ACTOR_TYPE_PLAYER,
	ACTOR_TYPE_ANIMAL,
	ACTOR_TYPE_STATIC_OBJECT_FOOD,
	ACTOR_TYPE_STATIC_OBJECT_WEAPON,
	ACTOR_TYPE_STATIC_OBJECT_CONTAINER,
	ACTOR_TYPE_STATIC_OBJECT_PROJECTILE,
	ACTOR_TYPE_STATIC_OBJECT_MATERIAL,
	ACTOR_TYPE_DYNAMIC_OBJECT_PROJECTILE,
	ACTOR_TYPE_DEAD_PLAYER,
	ACTOR_TYPE_DEAD_ANIMAL
};

class Actor;

class ActorPositionEvent : public Event
{
public:
	Actor *zActor;
};

class ActorRotationEvent : public Event
{
public:
	Actor *zActor;
};

class ActorScaleEvent : public Event
{
public:
	Actor *zActor;
};

class ActorDeleteEvent : public Event
{
public:
	Actor* zActor;
};

class ActorDirEvent : public Event
{
public:
	Actor *zActor;
};

/* 
   Every time an actor is created, an ID will be generated in this class. This id should be used for this actor.
   An exception is Players. They have the same ID as the ClientChannel.
*/
class Actor : public Observed
{
	Vector3 zPos;
	Vector4 zRot;
	Vector3 zDir;
	Vector3 zScale;

	long zID;
	PhysicsObject* zPhysicsObject;

public:
	Actor();
	virtual ~Actor();
	
	inline long GetID() const { return this->zID; }

	// Set Transformation Functions
	void SetPosition(const Vector3& pos);
	void SetRotation(const Vector4& rot);
	void SetRotation(const Vector3& around, const float angle);
	void SetPhysicsObject( PhysicsObject* object );
	void SetScale(const Vector3& scale);
	void SetDir(const Vector3& dir);

	// Get Transformation Functions
	inline const Vector3& GetPosition() const { return zPos; }
	inline const Vector4& GetRotation() const { return zRot; }
	inline const Vector3& GetScale() const { return zScale; }
	inline const Vector3& GetDir() const { return zDir; }
	const std::string& GetModel() const;
};