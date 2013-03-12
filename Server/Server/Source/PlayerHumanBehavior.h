#pragma once
#include "PlayerBehavior.h"
#include "PlayerConfigReader.h"

class PlayerHumanBehavior : public PlayerBehavior
{
private:

	float zIntervalCounter;
	PlayerConfigReader* zPlayerConfigReader;

public:
	PlayerHumanBehavior(Actor* actor, World* world, Player* player);
	virtual ~PlayerHumanBehavior();
	

	virtual bool Update(float dt);
	bool PhysicalConditionCalculator(float dt); //The function in which health, stamina, hunger and thirst is slightly changed in relation to each other. Includes health regeneration


};