#pragma once

#include "Behavior.h"
#include <Vector.h>
#include "Observer.h"



class ProjectileArrowBehavior : public Behavior, public Observed
{
private:
	Vector3 zVelocity;
	float zDamping;
	float zSpeed;
	float zLength;
	float zCollisionRadius;
	bool zMoving;

	unsigned int zNearActorsIndex;
	std::set<Actor*> zNearActors;

public:
	ProjectileArrowBehavior(Actor* actor, World* world);

	bool Update(float dt);
	bool IsMoving() {return this->zMoving;}
	void SetSpeed(float speed) {this->zSpeed = speed;}
	void SetDamping(float damping) {this->zDamping = damping;}
 	void SetNearActors(std::set<Actor*> actors);
 
 	virtual Actor* CheckCollision();
 	bool RefreshNearCollideableActors(const std::set<Actor*>& actors);

	/*! Stops the movement.*/
	inline void Stop(){zMoving = false;}
	inline float GetLength() const {return zLength;}
	inline std::set<Actor*> GetNearActors() const { return this->zNearActors;}
	float GetCollisionRadius() const { return zCollisionRadius; }

};