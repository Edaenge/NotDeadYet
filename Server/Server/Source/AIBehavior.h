//Simon Edänge 2013-02-24

#pragma once

#include <World/World.h>
#include "Behavior.h"
#include "Actor.h"
#include "AI.h"

class AIBehavior : public Behavior
{
public:
	AIBehavior(Actor* actor, World* world);
	virtual ~AIBehavior();
protected:
	
private:
};