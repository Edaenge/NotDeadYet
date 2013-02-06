#pragma once
#include "Behavior.h"

class ProjectileBehavior : public Behavior
{
private:

public:
	ProjectileBehavior(Actor* actor, World* world);

	bool Update(float dt);
};