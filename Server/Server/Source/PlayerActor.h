/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "BioActor.h"
#include <World/WorldEvents.h>
#include "PlayerEvent.h"

class Game;
/*This class is used to save player information such as position and states.
  This information is sent to clients.
*/
class PlayerActor : public BioActor
{
	Vector3 zInitialDirection;
	float zFullness;
	float zHydration;

	float zFullnessMax;
	float zHydrationMax;

	bool zExhausted;
	bool zHasSprinted;

public:
	PlayerActor(Player* player, PhysicsObject* pObj, Observer* game);

	virtual ~PlayerActor();

	float GetFullness();
	float GetHydration();

	inline float GetFullnessMax() {return this->zFullnessMax;}
	inline float GetHydrationMax() {return this->zHydrationMax;}
	inline bool GetExhausted(){ return this->zExhausted; }
	inline bool GetHasSprinted(){ return this->zHasSprinted; }

	void SetFullness(float fullness, const bool notify = true);
	void SetHydration(float hydration, const bool notify = true);
	void SetExhausted(bool exhausted);
	inline void SetHasSprinted(bool hasSprinted) { this->zHasSprinted = hasSprinted; }
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