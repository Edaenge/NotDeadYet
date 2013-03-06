#include "ProjectileActor.h"


ProjectileActor::ProjectileActor(Actor* owner, PhysicsObject* pObj) :
	zOwner(owner), Actor()
{
	this->SetPhysicsObject(pObj);	
	this->zType = ACTOR_TYPE_PROJECTILE;
}

ProjectileActor::~ProjectileActor()
{

}

