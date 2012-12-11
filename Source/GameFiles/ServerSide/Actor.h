/*
Made by Edänge Simon Datum(30/11/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/
#pragma once

#include <vector.h>

/* This is an abstract base class for objects in the game.
   Every time an actor is created, an ID will be generated in this class. This id should be used for this actor.
   An exception is Players. They have the same ID as the ClientChannel.
*/


class Actor
{
public:
	Actor()
	{
		this->zID = this->zNextAID;
		this->zNextAID++;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
	}
	Actor(const Vector3& pos)
	{
		this->zID = this->zNextAID;
		this->zNextAID++;

		this->zPos = pos;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
	}
	Actor(const Vector3& pos, const Vector4& rot) 
	{
		this->zID = this->zNextAID;
		this->zNextAID++;

		this->zPos = pos; 
		this->zRot = rot;
		this->zScale = Vector3(0.05f,0.05f,0.05f);
	}
	virtual ~Actor(){}
	
	inline const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}
	inline int  GetID() const {return this->zID;}

	void SetPosition(const Vector3& pos) {zPos = pos;}
	void SetRotation(const Vector4& rot) {zRot = rot;}
	void SetScale(const Vector3& scale) {zScale = scale;}
	void SetID(const int id) {this->zID = id;}

	virtual void Update(float deltaTime) = 0;

protected:
	int zID;
	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;
private:
	static long zNextAID;
	
};