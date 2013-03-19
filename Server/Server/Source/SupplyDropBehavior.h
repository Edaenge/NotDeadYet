//Made By Simon Edänge 2013-03-10

#pragma once

#include "Behavior.h"
#include <Vector.h>

class World;
class SoundHandler;

/*
	This behavior will make the object fall from actor position
	and land on destination.
*/
class SupplyDropBehavior : public Behavior, public Observed
{
public:
	SupplyDropBehavior(Actor* actor, World* world, Vector2& destination, SoundHandler* soundHandler);
	virtual ~SupplyDropBehavior();

	virtual bool Update(float dt);
	void Stop() {this->zMoving = false;}
	bool IsMoving() {return this->zMoving;}

private:
	SoundHandler* zSoundHandler;
	bool zMoving;
	bool zSupplyCrateLanded;
	Vector3 zDestination;
	Vector3 zVelocity;
};

