/*
Made by Edänge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "../../../../../Source/GameFiles/AnimationStates.h"
#include <string>
#include "../../../../../Source/PhysicsEngine/PhysicsObject.h"

/*This class is Abstract, this class is used to create living creatures such as humans, animals etc. */
class BioActor : public Actor
{
public:

	BioActor();
	BioActor(const Vector3& startPos);
	BioActor(const Vector3& startPos, const Vector4& rot);
	virtual ~BioActor();

	virtual void Update(float deltaTime) = 0;
	/*! Returns true if BioActor dies from the damage done.*/
	virtual bool TakeDamage(const float dmg);
	/*! Returns false if player cannot sprint, due to stamina.*/
	virtual bool Sprint(float dt);
	virtual bool IsAlive() const;
	/*! Rewinds to previous position.*/
	virtual void RewindPosition();
	/*! Check if the player has moved after last update.
		Note that it may change if the player collides with an other Actor.
	*/
	virtual bool HasMoved();

	int GetState() const {return this->zState;}
	float GetVelocity() const {return this->zVelocity;}
	float GetStamina() const {return this->zStamina;}
	float GetHealth() const {return this->zHealth;}
	inline PhysicsObject* GetPhysicObject() const {return this->zPhysicObj;}

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(const int state){this->zState = state;}
	void SetVelocity(const float velocity){this->zVelocity = velocity;}
	void SetHealth(const float health){this->zHealth = health;}
	void SetStamina(const float stamina){this->zStamina = stamina;}
	void SetPhysicObj(PhysicsObject* pObj){this->zPhysicObj = pObj;}
	/*! Sets Actor position and physical object position.*/
	inline virtual void SetPosition(const Vector3& pos)
	{
		this->zPos = pos;
		if(zPhysicObj)
			this->zPhysicObj->SetPosition(pos);
	}
	/*! Sets Actor scale and physical object scale.*/
	virtual void SetScale(const Vector3& scale)
	{
		this->zScale = scale;
		if(zPhysicObj)
			this->zPhysicObj->Scale(scale);
	}
	/*! Sets Actor rotation and physical object rotation.*/
	inline virtual void SetRotation(const Vector4& rot)
	{
		this->zRot = rot;
		if(zPhysicObj)
			this->zPhysicObj->SetQuaternion(rot);
	}


private:
	void InitValues();
	

protected:
	int		zState;
	float	zVelocity;

	float	zHealth;
	float	zHealthMax;

	float	zStamina;
	float	zStaminaMax;
	float	zStaminaCof;

	bool	zStaminaChanged;
	bool	zHealthChanged;

	bool	zAlive;

	Vector3 zPreviousPos;

	PhysicsObject* zPhysicObj;



};