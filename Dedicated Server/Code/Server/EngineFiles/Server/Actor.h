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
	}
	Actor(const Vector3& pos)
	{
		this->zID = -1;

		this->zPos = pos;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
		this->zActorModel = "none";
		this->zActorObjectName = "none";
	}
	Actor(const Vector3& pos, const Vector4& rot) 
	{
		this->zID = -1;

		this->zPos = pos; 
		this->zRot = rot;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
		this->zActorModel = "none";
		this->zActorObjectName = "none";
	}
	virtual ~Actor(){this->zNextAID = 0;}
	
	inline const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}
	inline int  GetID() const {return this->zID;}
	std::string GetActorModel() const {return zActorModel;}
	std::string GetActorObjectName() const {return zActorObjectName;}

	void SetPosition(const Vector3& pos) {zPos = pos;}
	void SetRotation(const Vector4& rot) {zRot = rot;}
	void SetScale(const Vector3& scale) {zScale = scale;}
	void SetID(const int id) {this->zID = id;}
	void SetActorModel(const std::string& modelStr) {zActorModel = modelStr;}
	void SetActorObjectName(const std::string& objectStr) {zActorObjectName = objectStr;}

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
	std::string zActorModel;
	std::string zActorObjectName;
private:
	static long zNextAID;
	int zID;
	
};