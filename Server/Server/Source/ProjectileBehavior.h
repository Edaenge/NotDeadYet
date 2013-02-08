#pragma once

#include "Behavior.h"
#include <Vector.h>

class ProjectileBehavior : public Behavior
{
private:
	Vector3 zVelocity;
	float zDamping;
	bool zMoving;

public:
	ProjectileBehavior(Actor* actor, World* world);

	bool Update(float dt);
	bool IsMoving() {return this->zMoving;}
};