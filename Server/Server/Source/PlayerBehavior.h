#pragma once
#include "Behavior.h"

class Player;

class PlayerBehavior : public Behavior
{
private:

public:
	PlayerBehavior(Actor* actor, World* world, Player* player);
	virtual	~PlayerBehavior();
	virtual bool Update(float dt);

	bool ProcessClientData(Vector3 direction, Vector4 rotation);

protected:
	Player* zPlayer;
	Vector3 zVelocity;
};