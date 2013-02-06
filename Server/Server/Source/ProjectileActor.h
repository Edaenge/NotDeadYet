#pragma once

#include "Actor.h"

class ProjectileActor : public Actor
{
	Actor* zShooter;
public:
	ProjectileActor( Actor* shooter );
	virtual ~ProjectileActor();

protected:


private:

};