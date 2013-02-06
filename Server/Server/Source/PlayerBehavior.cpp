#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"

PlayerBehavior::PlayerBehavior(Actor* actor, World* world, WorldAnchor* zAnchor, Player* player) : Behavior(actor, world, anchor)
{	this->zPlayer = player;
	this->zVelocity = Vector3(0, 0, 0);
}

PlayerBehavior::~PlayerBehavior()
{

}

bool PlayerBehavior::ProcessClientData( Vector3 direction, Vector4 rotation )
{
	this->zActor->SetDir(direction);
	this->zActor->SetRotation(rotation);

	return true;
}

