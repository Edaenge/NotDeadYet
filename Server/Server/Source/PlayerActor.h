/*
Made by Edänge Simon Datum(30/11/12 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "BioActor.h"
#include "KeyValues.h"
#include "KeyStates.h"
#include "Player.h"
#include <World/WorldEvents.h>

/*This class is used to save player information such as position and states.
  This information is sent to clients.
*/
class PlayerActor : public BioActor
{
	Vector3 zInitialDirection;
	Player* zPlayer;

public:
	PlayerActor(Player* player, PhysicsObject* pObj);

	virtual ~PlayerActor();

	float GetFullness();
	float GetHydration();

	float GetFullnessMax() {return this->zFullnessMax;}
	float GetHydrationMax() {return this->zHydrationMax;}

	void SetFullness(float fullness);
	void SetHydration(float hydration);

	void HungerHasChanged() {this->zHungerChanged = true;}
	void HydrationHasChanged() {this->zHydrationChanged = true;}

	Player* GetPlayer(){ return this->zPlayer; }
	void SetPlayer(Player* player) {this->zPlayer = player;}
	/*! Returns The dropped item.
	Doesn't deallocate Item.*/
	Item* DropItem(const long ID);

private:

	float zFullness;
	float zHydration;
	
	bool zHydrationChanged;
	bool zHungerChanged;

	float zFullnessMax;
	float zHydrationMax;
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