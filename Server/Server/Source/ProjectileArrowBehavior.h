#pragma once

#include "Behavior.h"
#include <Vector.h>
#include <World/Observer.h>



class ProjectileArrowBehavior : public Behavior, public Observed
{
private:
	Vector3 zVelocity;
	float zDamping;
	float zSpeed;
	bool zMoving;

public:
	ProjectileArrowBehavior(Actor* actor, World* world, float speed = 35.0f);
	virtual ~ProjectileArrowBehavior();

	bool Update(float dt);
	bool IsMoving() {return this->zMoving;}
	void SetSpeed(float speed) {this->zSpeed = speed;}
	void SetDamping(float damping) {this->zDamping = damping;}

	/*! Stops the movement.*/
	inline void Stop(){zMoving = false;}
	float GetLength() const {return zLength;}

	/*! Check collisions using ray vs mesh. Ignores owner*/
	virtual Actor* RayVsMeshCollision( const Actor* actor, const Vector3& rayOrigin, float& range, const std::set<Actor*>& actors );

private:
	float zLength;

};