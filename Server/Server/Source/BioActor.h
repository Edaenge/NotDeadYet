/*
Made by Ed�nge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska h�gskola.
*/

#pragma once

#include "Actor.h"
#include <AnimationStates.h>
#include <string>
#include "Damage.h"
#include "Inventory.h"

class BioActorTakeDamageEvent : public Event
{
public:
	Actor* zActor;
	Damage zDamage;
	Actor* zDealer;
};

/*This class is Abstract, this class is used to create living creatures such as humans, animals etc. */
class BioActor : public Actor
{
public:
	BioActor(const Vector3& startPos, const Vector4& rot);
	BioActor();

	virtual ~BioActor();

	// virtual void Update(float deltaTime) = 0;

	/*! Returns true if BioActor dies from the damage done.*/
	virtual bool TakeDamage(const Damage& dmg, Actor* dealer);

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

	int GetState() const {return this->zState;}
	float GetVelocity() const {return this->zVelocity;}
	float GetStamina() const {return this->zStamina;}
	float GetHealth() const {return this->zHealth;}
	Vector3 GetDirection() const {return this->zDirection;}
	float GetStaminaMax() const {return this->zStaminaMax;}
	float GetStaminaCof() const {return this->zStaminaCof;}
	float GetHealthMax() const {return this->zHealthMax;}

	bool  IsBleeding() const {return this->zBleeding;}

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(const int state) {this->zState = state;}
	void SetVelocity(const float velocity) {this->zVelocity = velocity;}
	void SetHealth(const float health) {this->zHealth = health; this->zHealthChanged = true;}
	void SetStamina(const float stamina) {this->zStamina = stamina; this->zStaminaChanged = true;}
	void SetDirection(const Vector3& dir) {this->zDirection = dir;}
	/*! Returns Pos, Rot, Scale, Stamina, Health, State.*/
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);
	void SetStaminaMax(const float max) {this->zStaminaMax = max;}
	void SetStaminaCof(const float cof) {this->zStaminaCof = cof;}

	void SetBleeding(const bool isBleeding) {this->zBleeding = isBleeding;}

	void StaminaHasChanged() {this->zStaminaChanged = true;}
	void HealthHasChanged() {this->zHealthChanged = true;}


protected:
	int		zState;
	float	zVelocity;

	float	zHealth;
	float	zHealthMax;

	float	zStamina;
	float	zStaminaMax;
	float	zStaminaCof;

	bool	zBleeding;

	bool	zStaminaChanged;
	bool	zHealthChanged;

	bool	zAlive;

	Vector3 zPreviousPos;
	Vector3 zDirection;

	Inventory* zInventory;
};