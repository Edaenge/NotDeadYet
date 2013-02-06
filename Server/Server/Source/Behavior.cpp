#include "Behavior.h"

Behavior::Behavior( Actor* actor, World* world, WorldAnchor* anchor)
{
	this->zActor = actor;
	this->zWorld = world;
	this->zAnchor = anchor;
}