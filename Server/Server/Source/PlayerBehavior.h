#pragma once

#include "Behavior.h"
#include <Vector.h>

class Player;

const float SIGHTRADIUS = 200.0f;

class PlayerBehavior : public Behavior
{
public:
	PlayerBehavior(Actor* actor, World* world, Player* player);
	virtual	~PlayerBehavior();

	bool ProcessClientData(Vector3 direction, Vector4 rotation);

protected:
	Player* zPlayer;
	Vector3 zVelocity;
};