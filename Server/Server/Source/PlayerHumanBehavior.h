#pragma once
#include "PlayerBehavior.h"

class PlayerHumanBehavior : public PlayerBehavior
{
private:

public:
	PlayerHumanBehavior(Actor* actor, World* world, WorldAnchor* anchor, Player* player);
	virtual ~PlayerHumanBehavior();

	virtual bool Update(float dt);
};