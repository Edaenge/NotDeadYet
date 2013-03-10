//Made By Simon Edänge 2013-03-10

#pragma once

#include "Behavior.h"
#include <Vector.h>

class World;

/*
	This behavior will make the object fall from actor position
	and land on destination.
*/
class SupplyDropBehavior : public Behavior
{
public:
	SupplyDropBehavior(Actor* actor, World* world, Vector2& destination);
	virtual ~SupplyDropBehavior();

	virtual bool Update(float dt);
	bool Stop() {this->zMoving = false;}
	bool IsMoving() {return this->zMoving;}

private:
	float zSpeed;
	bool zMoving;
	Vector3 zDestination;
	Vector3 zVelocity;
};

