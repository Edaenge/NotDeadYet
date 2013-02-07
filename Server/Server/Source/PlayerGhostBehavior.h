#pragma once
#include "PlayerBehavior.h"

class PlayerGhostBehavior : public PlayerBehavior
{
private:

public:
	PlayerGhostBehavior(Actor* actor, World* world, Player* player);
	virtual ~PlayerGhostBehavior();

	virtual bool Update(float dt);
};