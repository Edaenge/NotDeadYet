/*
Made by Edänge Simon 2013-01-22
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "../../../../../Source/PhysicsEngine/PhysicsObject.h"
#include "../../../../../Source/Network/NetworkMessageConverter.h"

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
		if(this->zPhysicObj) 
			return this->zPhysicObj->GetPosition(); 

		Vector3 def;
		return def;
	}

	/*! Returns the model initial direction, the direction in model space.*/
	inline Vector3 GetInitialDirection() const {return zInitialDirection;}
	/*! PhysicsObject needs to be initialized.*/
	const Vector3 GetScale() const 
	{
		if(this->zPhysicObj) 
			return this->zPhysicObj->GetScaling(); 
		
		Vector3 def;
		return def;
	}
	/*! PhysicsObject needs to be initialized.*/
	inline const Vector4 GetRotation() const 
	{
		if(this->zPhysicObj) 
			return this->zPhysicObj->GetRotationQuaternion(); 

		Vector4 def;
		return def;
	}

	/*! PhysicsObject needs to be initialized.*/
	inline void SetPosition(const Vector3& pos) 
	{
		if(this->zPhysicObj) 
			this->zPhysicObj->SetPosition(pos);
	}
	/*! PhysicsObject needs to be initialized.*/
	inline void SetRotation(const Vector4& rot) 
	{
		if(this->zPhysicObj) 
			this->zPhysicObj->SetQuaternion(rot);
	}
	/*! PhysicsObject needs to be initialized.*/
	void SetScale(const Vector3& scale) 
	{
		if(this->zPhysicObj) 
		{
			this->zPhysicObj->SetScaling(scale);
			this->zScaleChanged = true;
		}
	}
	void SetPhysicObject(PhysicsObject* pObj)
	{
		this->zPhysicObj = pObj;
	}
	/*! */
	PhysicsObject* GetPhysicObject() const {return this->zPhysicObj;}

	void AddChangedData(string& mess, NetworkMessageConverter* nmc);

	virtual void Update(float deltaTime) = 0;

protected:
	PhysicsObject* zPhysicObj;
	/*! The models initial model direction, which direction the model is pointing in model space.*/
	Vector3 zInitialDirection;

private:
	bool zScaleChanged;
};