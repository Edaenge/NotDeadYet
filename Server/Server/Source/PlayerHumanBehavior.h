#pragma once
#include "PlayerBehavior.h"

class PlayerHumanBehavior : public PlayerBehavior
{
private:

	float zIntervalCounter;
	bool zExhausted;
	bool zHasSprinted;

public:
	PlayerHumanBehavior(Actor* actor, World* world, Player* player);
	virtual ~PlayerHumanBehavior();

	virtual bool Update(float dt);
	void PhysicalConditionCalculator(float dt); //The function in which health, stamina, hunger and thirst is slightly changed in relation to each other. Includes health regeneration

};