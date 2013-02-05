#pragma once
#include "Behavior.h"
class KeyState
{

};

class PlayerBehavior : public Behavior
{
private:
	KeyState* zKeyState;
public:
	PlayerBehavior(Actor* actor, World* world, KeyState* zKeyState);

	bool Update(float dt);
};