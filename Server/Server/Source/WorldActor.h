#pragma once

#include "Actor.h"

class WorldActor : public Actor
{
public:
	WorldActor( PhysicsObject* pObj, const float& blockRadius );
	virtual ~WorldActor();

};