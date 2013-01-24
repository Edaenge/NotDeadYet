/*
Made by Edänge Simon 2013-01-22
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "../../../../../Source/PhysicsEngine/PhysicsObject.h"

class PhysicsActor : public Actor
{
public:
	PhysicsActor();
	PhysicsActor(const Vector3& startpos, PhysicsObject* pObj);
	PhysicsActor(const Vector3& startpos, PhysicsObject* pObj, const Vector4& rot);

	virtual ~PhysicsActor();

	/*! PhysicsObject needs to be initialized.*/
	inline Vector3 GetPosition() const 
	{
		if(zPhysicObj) 
			return zPhysicObj->GetPosition(); 

		Vector3 def;
		return def;
	}
	/*! PhysicsObject needs to be initialized.*/
	const Vector3 GetScale() const 
	{
		if(zPhysicObj) 
			return zPhysicObj->GetScaling(); 
		
		Vector3 def;
		return def;
	}
	/*! PhysicsObject needs to be initialized.*/
	inline const Vector4 GetRotation() const 
	{
		if(zPhysicObj) 
			return zPhysicObj->GetRotationQuaternion(); 

		Vector4 def;
		return def;
	}

	/*! PhysicsObject needs to be initialized.*/
	inline void SetPosition(const Vector3& pos) 
	{
		if(zPhysicObj) 
			zPhysicObj->SetPosition(pos);
	}
	/*! PhysicsObject needs to be initialized.*/
	inline void SetRotation(const Vector4& rot) 
	{
		if(zPhysicObj) 
			zPhysicObj->SetQuaternion(rot);
	}
	/*! PhysicsObject needs to be initialized.*/
	void SetScale(const Vector3& scale) 
	{
		if(zPhysicObj) 
			zPhysicObj->SetScaling(scale);
	}
	void SetPhysicObject(PhysicsObject* pObj)
	{
		this->zPhysicObj = pObj;
	}
	/*! */
	PhysicsObject* GetPhysicObject() const {return this->zPhysicObj;}

	virtual void Update(float deltaTime) = 0;

protected:
	PhysicsObject* zPhysicObj;

private:

};