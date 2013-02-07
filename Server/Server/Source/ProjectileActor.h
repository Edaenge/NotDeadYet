#pragma once

#include "Actor.h"

class ProjectileActor : public Actor
{
	
public:
	ProjectileActor(Actor* owner, PhysicsObject* pObj);
	virtual ~ProjectileActor();

	Actor* GetOwner() {return this->zOwner;}

protected:
	Actor* zOwner;

private:

};