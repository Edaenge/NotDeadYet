#pragma once

#include "Behavior.h"
#include <Vector.h>

class ProjectileArrowBehavior : public Behavior
{
private:
	Vector3 zVelocity;
	float zDamping;
	float zSpeed;
	float zLength;
	bool zMoving;

public:
	ProjectileArrowBehavior(Actor* actor, World* world);

	bool Update(float dt);
	bool IsMoving() {return this->zMoving;}
	void SetSpeed(float speed) {this->zSpeed = speed;}
	void SetDamping(float damping) {this->zDamping = damping;}
	/*! Stops the movement.*/
	inline void Stop(){zMoving = false;}
	inline float GetLenght() const {return zLength;}
};