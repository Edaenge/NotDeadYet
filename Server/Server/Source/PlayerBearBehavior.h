#pragma once
#include "PlayerBehavior.h"

class PlayerBearBehavior : public PlayerBehavior
{
private:
	
public:
	PlayerBearBehavior(Actor* actor, World* world, Player* palyer);
	virtual ~PlayerBearBehavior();

	bool Update(float dt);
};