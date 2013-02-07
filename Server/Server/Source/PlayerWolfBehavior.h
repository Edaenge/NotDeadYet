#pragma once

#include "PlayerBehavior.h"


class PlayerWolfBehavior : public PlayerBehavior
{
private:

public:
	PlayerWolfBehavior(Actor* actor, World* world, Player* player);
	virtual ~PlayerWolfBehavior();

	virtual bool Update(float dt);
};