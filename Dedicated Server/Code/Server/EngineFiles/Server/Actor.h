/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
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
		this->zScale = Vector3(0.05f,0.05f,0.05f);
		this->zActorModel = "none";
		this->zActorObjectName = "none";
		this->zDirection = Vector3(0.0f, 0.0f, -1.0f);
		this->zUp = Vector3(0.0f, 1.0f, 0.0f);
	}
	Actor(const Vector3& pos)
	{
		this->zID = -1;

		this->zPos = pos;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
		this->zActorModel = "none";
		this->zActorObjectName = "none";
		this->zDirection = Vector3(0.0f, 0.0f, -1.0f);
		this->zUp = Vector3(0.0f, 1.0f, 0.0f);
	}
	Actor(const Vector3& pos, const Vector4& rot) 
	{
		this->zID = -1;

		this->zPos = pos; 
		this->zRot = rot;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
		this->zActorModel = "none";
		this->zActorObjectName = "none";
		this->zDirection = Vector3(0.0f, 0.0f, -1.0f);
		this->zUp = Vector3(0.0f, 1.0f, 0.0f);
	}
	virtual ~Actor(){}
	
	inline const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}
	inline int  GetID() const {return this->zID;}
	std::string GetActorModel() const {return zActorModel;}
	std::string GetActorObjectName() const {return zActorObjectName;}
	Vector3 GetDirection() const {return this->zDirection;}
	const Vector3& GetUpVector() const {return this->zUp;}

	inline virtual void SetPosition(const Vector3& pos) {zPos = pos;}
	inline virtual void SetRotation(const Vector4& rot) {zRot = rot;}
	virtual void SetScale(const Vector3& scale) {zScale = scale;}
	inline virtual void SetID(const int id) {this->zID = id;}
	virtual void SetActorModel(const std::string& modelStr) {zActorModel = modelStr;}
	virtual void SetActorObjectName(const std::string& objectStr) {zActorObjectName = objectStr;}
	inline virtual void SetDirection(Vector3 dir) {this->zDirection = dir;}
	virtual void SetUpVector(const Vector3& up){this->zUp = up;}

	virtual void Update(float deltaTime) = 0;
protected:
	void GenerateID()
	{
		this->zID = this->zNextAID; 
		this->zNextAID++;
	}
protected:
	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;
	Vector3 zDirection;
	Vector3 zUp;
	std::string zActorModel;
	std::string zActorObjectName;
private:
	static long zNextAID;
	int zID;
	
};