/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "BioActor.h"
#include <World/WorldEvents.h>
#include "PlayerEvent.h"

/*This class is used to save player information such as position and states.
  This information is sent to clients.
*/
class PlayerActor : public BioActor
{
	Vector3 zInitialDirection;

public:
	PlayerActor(Player* player, PhysicsObject* pObj);

	virtual ~PlayerActor();

	float GetFullness();
	float GetHydration();

	float GetFullnessMax() {return this->zFullnessMax;}
	float GetHydrationMax() {return this->zHydrationMax;}
	bool GetExhausted(){ return this->zExhausted; }
	bool GetHasSprinted(){ return this->zHasSprinted; }

	void SetFullness(float fullness, const bool notify = true);
	void SetHydration(float hydration, const bool notify = true);
	void SetExhausted(bool exhausted);
	void SetHasSprinted(bool hasSprinted){ this->zHasSprinted = hasSprinted; }

private:

	float zFullness;
	float zHydration;
	
	float zFullnessMax;
	float zHydrationMax;

	bool zExhausted;
	bool zHasSprinted;
};

//class PlayerUpdatedEvent : public Event
//{
//public:
//	PlayerActor* playerActor;
//	bool validMove;
//	Vector3 prevPos;
//	PlayerUpdatedEvent(PlayerActor* playerActor, bool& validMove, Vector3 prevPos) 
//		: playerActor(playerActor), validMove(validMove), prevPos(prevPos){}
//};