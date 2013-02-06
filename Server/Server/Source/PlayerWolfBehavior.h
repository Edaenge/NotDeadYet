#pragma once
#include "PlayerBehavior.h"

class PlayerWolfBehavior : public PlayerBehavior
{
private:

public:
	PlayerWolfBehavior(Actor* actor, World* world, WorldAnchor* anchor, Player* player);
	virtual ~PlayerWolfBehavior();

	virtual bool Update(float dt);
};