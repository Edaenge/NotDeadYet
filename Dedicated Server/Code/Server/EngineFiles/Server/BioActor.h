/*
Made by Edänge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "../../../../../Source/GameFiles/AnimationStates.h"
#include <string>

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

	int GetState() const {return this->zState;}
	float GetVelocity() const {return this->zVelocity;}
	const Vector3& GetDirection() const {return this->zDir;}
	const Vector3& GetUpVector() const {return this->zUp;}
	float GetStamina() const {return this->zStamina;}
	float GetHealth() const {return this->zHealth;}

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(const int state){this->zState = state;}
	void SetVelocity(const float velocity){this->zVelocity = velocity;}
	inline void SetDirection(const Vector3& dir){this->zDir = dir;}
	inline void SetUpVector(const Vector3& up){this->zUp = up;}
	void SetHealth(const float health){this->zHealth = health;}
	void SetStamina(const float stamina){this->zStamina = stamina;}

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

	Vector3 zDir;
	Vector3 zUp;



};