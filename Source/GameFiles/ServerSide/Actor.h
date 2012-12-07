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
	Actor(){}
	Actor(const Vector3& pos, const Vector4& rot) 
	{
		zPos = pos; 
		zRot = rot;
		zScale = Vector3(1.0f,1.0f,1.0f);
	}
	virtual ~Actor(){}
	
	inline const Vector3& GetPosition() const {return zPos;}
	inline const Vector3& GetScale() const {return zScale;}
	inline const Vector4& GetRotation() const {return zRot;}

	inline void SetPosition(const Vector3& pos) {zPos = pos;}
	inline void SetRotation(const Vector4& rot) {zRot = rot;}
	inline void SetScale(const Vector3& scale) {zScale = scale;}

	virtual void Update(float deltaTime) = 0;

protected:
	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;
};