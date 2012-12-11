/*
Made by Edänge Simon Datum(30/11/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/
#pragma once

#include <vector.h>

/* This is an abstract base class for objects in the game.*/
class Actor
{
public:
	Actor(const int ID)
	{
		this->zID = ID;
		this->zScale = Vector3(0.05,0.05,0.05);
	}
	Actor(const int ID, const Vector3& pos)
	{
		this->zID = ID;
		this->zPos = pos;
		this->zScale = Vector3(0.05,0.05,0.05);
	}
	Actor(const int ID, const Vector3& pos, const Vector4& rot) 
	{
		this->zPos = pos; 
		this->zRot = rot;
		this->zScale = Vector3(0.05,0.05,0.05);
		this->zID = ID;
	}
	virtual ~Actor(){}
	
	inline const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}
	inline int  GetID() const {return this->zID;}

	void SetPosition(const Vector3& pos) {zPos = pos;}
	void SetRotation(const Vector4& rot) {zRot = rot;}
	void SetScale(const Vector3& scale) {zScale = scale;}
	void SetID(const int id) {this->zID;}

	virtual void Update(float deltaTime) = 0;

protected:
	int zID;
	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;
	
};