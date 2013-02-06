#pragma once
#include "Behavior.h"
#include "KeyStates.h"

class PlayerBehavior : public Behavior
{
private:
	KeyStates* zKeyState;
public:
	PlayerBehavior(Actor* actor, World* world, KeyStates* zKeyState);

	bool Update(float dt);
};