/*
Made by Edänge Simon 2012-12-11
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "Damage.h"
#include "Player.h"
#include "Inventory.h"
#include <AnimationStates.h>
#include <string>
#include <chrono>


class BioActorTakeDamageEvent : public Event
{
public:
	virtual ~BioActorTakeDamageEvent() {}

	Actor* zActor;
	Damage* zDamage;
	Actor* zDealer;
};

class BioActorStateEvent : public Event
{
public:
	virtual ~BioActorStateEvent() {}

	Actor* zBioActor;
};

class BioActorPhysicalConditionHealthEvent : public Event
{
public:
	virtual ~BioActorPhysicalConditionHealthEvent() {}

	Actor* zBioActor;
};

class BioActorPhysicalConditionStaminaEvent : public Event
{
public:
	virtual ~BioActorPhysicalConditionStaminaEvent() {}

	Actor* zBioActor;
};

class BioActorPhysicalConditionBleedingEvent : public Event
{
public:
	virtual ~BioActorPhysicalConditionBleedingEvent() {}

	Actor* zBioActor;
};


/*This class is Abstract, this class is used to create living creatures such as humans, animals etc. */
class BioActor : public Actor
{
protected:
	int		zState;
	float	zVelocity;

	float	zHealth;
	float	zHealthMax;

	float	zStamina;
	float	zStaminaMax;
	float	zStaminaCof;

	float	zBleedingLevel;

	bool	zAlive;

	Vector3 zDirection;
	Vector3 zCameraOffset;
	Inventory* zInventory;

	Player* zPlayer;

	std::string zCurrentAction;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> zActionTimeStarted;
	std::chrono::microseconds zActionTimeLength;

public:
	bool zValid;

	BioActor();
	BioActor(Player* player, Observer* game);
	BioActor(const Vector3& startPos, const Vector4& rot);
	virtual ~BioActor();

	void SetAction(const std::string& actionName, float seconds);
	const std::string& GetCurrentAction();
	bool InAction();

	// virtual void Update(float deltaTime) = 0;

	/*! Returns true if BioActor dies from the damage done.*/
	virtual bool TakeDamage(Damage& dmg, Actor* dealer);

	/*! Returns false if player is dead.*/
	virtual bool IsAlive() const;
	/*! Check if the player has moved after last update.
		Note that it may change if the player collides with an other Actor.
	*/
	virtual bool HasMoved();
	/*! Kills the actor.*/
	virtual void Kill();
	/*! Adds a message string with health,stamina,hunger, hydration to mess.
	    This function checks if the data has changed since last update.
		If it has changed, we need to send it to the client.
		The parameter string is a network message string.
	*/

	int	GetState() const {return this->zState;}
	float GetVelocity() const {return this->zVelocity;}
	float GetStamina() const {return this->zStamina;}
	float GetHealth() const {return this->zHealth;}
	float GetStaminaMax() const {return this->zStaminaMax;}
	float GetStaminaCof() const {return this->zStaminaCof;}
	float GetHealthMax() const {return this->zHealthMax;}

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(const int state, const bool notify = true);
	void SetVelocity(const float velocity) {this->zVelocity = velocity;}
	void SetHealth(const float health, const bool notify = true); 
	void SetStamina(const float stamina, const bool notify = true); 
	void SetStaminaMax(const float max) {this->zStaminaMax = max;}
	void SetStaminaCof(const float cof) {this->zStaminaCof = cof;}

	void SetBleeding(const float levelBleeding, const bool notify = true);
	inline float GetBleeding() const {return this->zBleedingLevel;}
	Inventory* GetInventory() const {return this->zInventory;}
	Vector3 GetCameraOffset() {return this->zCameraOffset;}
	void SetCameraOffset(Vector3 offset) {this->zCameraOffset = offset;}

	Player* GetPlayer(){ return this->zPlayer; }
	void SetPlayer(Player* player) {this->zPlayer = player;}
};