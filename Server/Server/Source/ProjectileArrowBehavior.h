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
	float zNearByRadius;
	bool zMoving;

	unsigned int zNearActorsIndex;

	std::set<Actor*> zNearActors;
	std::set<Actor*> zNearWorldActors;
	std::set<Actor*> zNearBioActors;

public:
	ProjectileArrowBehavior(Actor* actor, World* world);

	bool Update(float dt);
	bool IsMoving() {return this->zMoving;}
	void SetSpeed(float speed) {this->zSpeed = speed;}
	void SetDamping(float damping) {this->zDamping = damping;}
 	void SetNearBioActors(std::set<Actor*> actors);
	void SetNearWorldActors(std::set<Actor*> actors);
 
 	bool RefreshNearCollideableActors(const std::set<Actor*>& actors);

	/*! Stops the movement.*/
	inline void Stop(){zMoving = false;}
	inline std::set<Actor*> GetNearActors() const { return this->zNearActors;}
	float GetNearByRadius() const { return zNearByRadius; }
	float GetLength() const {return zLength;}

	/*! Check collisions using ray vs mesh.*/
	virtual Actor* CheckCollision( const Vector3& pos, const float& radius, const std::set<Actor*>& actors);

protected:
	virtual Actor* CheckBioActorCollision();
	virtual Actor* CheckWorldActorCollision();

private:
	float zLength;

};