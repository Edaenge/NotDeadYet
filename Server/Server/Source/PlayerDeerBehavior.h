#pragma once
#include "PlayerBehavior.h"

class PlayerDeerBehavior : public PlayerBehavior
{
private:

public:
	PlayerDeerBehavior(Actor* actor, World* world, WorldAnchor* anchor, Player* palyer);
	virtual ~PlayerDeerBehavior();

	bool Update(float dt);
};