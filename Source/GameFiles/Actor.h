/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/
#pragma once

#include <vector.h>

/* This is an abstract base class for objects in the game.*/
class Actor
{
public:
	Actor(){}
	Actor(const Vector3& pos, const Vector4& rot) {zPos = pos; zRot = rot;}
	virtual ~Actor(){}
	
	const Vector3& GetPosition() const {return zPos;}
	const Vector3& GetScale() const {return zScale;}
	const Vector4& GetRotation() const {return zRot;}

	void SetPosition(const Vector3& pos) {zPos = pos;}
	void SetRotation(const Vector4& rot) {zRot = rot;}
	void SetScale(const Vector3& scale) {zScale = scale;}

	virtual void Update() = 0;

protected:
	Vector3 zPos;
	Vector3 zScale;
	Vector4 zRot;
};