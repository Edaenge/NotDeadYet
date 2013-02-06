#pragma once

#include "World/World.h"
class Actor // For testing
{

};


class Behavior
{
protected:
	Actor* zActor;
	World* zWorld;
public:
	Behavior(Actor* actor, World* world);
	virtual ~Behavior();

	virtual bool Update(float dt);

};