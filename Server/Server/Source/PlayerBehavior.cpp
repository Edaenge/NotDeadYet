#include "PlayerBehavior.h"


PlayerBehavior::PlayerBehavior( Actor* actor, World* world, KeyState* zKeyState ) : Behavior(actor, world)
{
	this->zKeyState = zKeyState;
}

bool PlayerBehavior::Update( float dt )
{
	Vector3 pos = this->zActor->GetPosition();
	Vector3 normal = this->zWorld->CalcNormalAt(pos.GetXZ());
	float groundYPos = this->zWorld->CalcHeightAtWorldPos(pos.GetXZ());

	// TODO: Check normal if its too steep.
	// Check it try to move down or up.
	// Make fall down.
	// See if below ground.
	return true;
}