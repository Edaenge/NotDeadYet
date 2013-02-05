#include "PlayerBehavior.h"


PlayerBehavior::PlayerBehavior( Actor* actor, World* world, KeyState* zKeyState ) : Behavior(actor, world)
{
	this->zKeyState = zKeyState;
}

bool PlayerBehavior::Update( float dt )
{

	return true;
}