#pragma once
#include "PlayerBehavior.h"

class PlayerDeerBehavior : public PlayerBehavior
{
private:
	float jumpingTime;
public:
	PlayerDeerBehavior(Actor* actor, World* world, Player* palyer);
	virtual ~PlayerDeerBehavior();

	bool Update(float dt);
};