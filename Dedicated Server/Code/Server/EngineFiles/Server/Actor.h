/*
Made by Ed�nge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska h�gskola.
*/
#pragma once

#include "../../../../../Source/MaloWLib/Safe.h"
#include <string>
#include "../../../../../External/NDYGFX/Include/Vector.h"

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
		this->zDirection = Vector3(0.0f, 0.0f, -1.0f);
		this->zUp = Vector3(0.0f, 1.0f, 0.0f);
	}

	virtual ~Actor(){}
	
	inline long  GetID() const {return this->zID;}
	std::string GetActorModel() const {return zActorModel;}
	std::string GetActorObjectName() const {return zActorObjectName;}
	Vector3 GetDirection() const {return this->zDirection;}
	const Vector3& GetUpVector() const {return this->zUp;}

	inline  void SetID(const long id) {this->zID = id;}
	virtual void SetActorModel(const std::string& modelStr) {zActorModel = modelStr;}
	virtual void SetActorObjectName(const std::string& objectStr) {zActorObjectName = objectStr;}
	inline  void SetDirection(Vector3 dir) {this->zDirection = dir;}
	virtual void SetUpVector(const Vector3& up){this->zUp = up;}

	virtual void Update(float deltaTime) = 0;
protected:
	void GenerateID()
	{
		this->zID = this->zNextAID; 
		this->zNextAID++;
	}
protected:
	Vector3 zDirection;
	Vector3 zUp;
	std::string zActorModel;
	std::string zActorObjectName;
private:
	static long zNextAID;
	long zID;
	
};