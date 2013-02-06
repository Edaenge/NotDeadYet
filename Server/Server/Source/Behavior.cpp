#include "Behavior.h"

Behavior::Behavior( Actor* actor, World* world )
{
	this->zActor = actor;
	this->zWorld = world;
}

bool Behavior::Update( float dt )
{
	return true;
}