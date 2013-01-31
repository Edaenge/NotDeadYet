/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include "Safe.h"
#include <string>
#include "Vector.h"

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

/* This is an abstract base class for objects in the game.
   Every time an actor is created, an ID will be generated in this class. This id should be used for this actor.
   An exception is Players. They have the same ID as the ClientChannel.
*/
class Actor
{
public:
	Actor()
	{
		this->zID = -1;
		this->zActorModel = "none";
		this->zActorObjectName = "none";
		this->zUp = Vector3(0.0f, 1.0f, 0.0f);
		this->zActorType = ACTOR_TYPE_NONE;
	}

	virtual ~Actor(){}
	
	inline long GetID() const {return this->zID;}
	std::string GetActorModel() const {return zActorModel;}
	std::string GetActorObjectName() const {return zActorObjectName;}
	const Vector3& GetUpVector() const {return this->zUp;}
	const unsigned int GetActorType() const {return zActorType;}

	inline  void SetID(const long id) {this->zID = id;}
	virtual void SetActorModel(const std::string& modelStr) {zActorModel = modelStr;}
	virtual void SetActorObjectName(const std::string& objectStr) {zActorObjectName = objectStr;}
	virtual void SetUpVector(const Vector3& up){this->zUp = up;}

	virtual void Update(float deltaTime) = 0;

protected:
	void GenerateID()
	{
		this->zID = this->zNextAID; 
		this->zNextAID++;
	}
protected:
	Vector3 zUp;
	std::string zActorModel;
	std::string zActorObjectName;
	unsigned int zActorType;
private:
	static long zNextAID;
	long zID;

};