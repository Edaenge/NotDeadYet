/*
Made by Jensen Christopher Datum(01/02/13 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "Player.h"
#include "KeyValues.h"
#include "KeyStates.h"
#include "NetworkMessageConverter.h"

class GhostActor : public Actor
{
public:
	GhostActor(Player* player);
	virtual ~GhostActor();

	Player* GetPlayer(){ return this->zPlayer; }
	void SetPlayer(Player* player) {this->zPlayer = player;}

private:
	/*float zVelocity;
	float maxHeight;*/
	//float zEnergy;

	Player* zPlayer;
};