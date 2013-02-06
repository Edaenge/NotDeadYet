#pragma once
#include "Behavior.h"
#include "KeyStates.h"

static const int MAX_VELOCITY = 20;
static const Vector3 GRAVITY = Vector3(0, -9.82f, 0);
static const float ELASTICITY = 0.5f;

class PlayerBehavior : public Behavior
{
private:
	KeyStates* zKeyState;
	Vector3 zVelocity;
public:
	PlayerBehavior(Actor* actor, World* world, KeyStates* zKeyState);

	bool Update(float dt);
};