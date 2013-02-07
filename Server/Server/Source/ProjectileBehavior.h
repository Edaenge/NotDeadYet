#pragma once

#include "Behavior.h"
#include <Vector.h>

static const Vector3 GRAVITY = Vector3(0, -9.82f, 0);

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