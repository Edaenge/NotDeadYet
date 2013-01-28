/*
Made by Edänge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "PhysicsActor.h"
#include "../../../../../Source/GameFiles/AnimationStates.h"
#include <string>
#include "../../../../../Source/PhysicsEngine/PhysicsObject.h"

/*This class is Abstract, this class is used to create living creatures such as humans, animals etc. */
class BioActor : public PhysicsActor
{
public:

	BioActor();
	BioActor(const Vector3& startPos, PhysicsObject* pObj);
	BioActor(const Vector3& startPos, PhysicsObject* pObj, const Vector4& rot);
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
	/*! Adds a message string with health,stamina,hunger, hydration to mess.
	    This function checks if the data has changed since last update.
		If it has changed, we need to send it to the client.
		The parameter string is a network message string.
	*/
	void AddChangedHData(string& mess, NetworkMessageConverter* nmc);

	int GetState() const {return this->zState;}
	float GetVelocity() const {return this->zVelocity;}
	float GetStamina() const {return this->zStamina;}
	float GetHealth() const {return this->zHealth;}
	Vector3 GetDirection() const {return this->zDirection;}
	inline PhysicsObject* GetPhysicObject() const {return this->zPhysicObj;}

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(const int state) {this->zState = state;}
	void SetVelocity(const float velocity) {this->zVelocity = velocity;}
	void SetHealth(const float health) {this->zHealth = health; this->zHealthChanged = true;}
	void SetStamina(const float stamina) {this->zStamina = stamina; this->zStaminaChanged = true;}
	void SetDirection(const Vector3& dir) {this->zDirection = dir;}

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
	Vector3 zDirection;
};