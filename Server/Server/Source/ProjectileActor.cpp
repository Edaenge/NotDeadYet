#include "ProjectileActor.h"


ProjectileActor::ProjectileActor(Actor* owner, PhysicsObject* pObj) :
	zOwner(owner), Actor()
{
	this->SetPhysicsObject(pObj);
	this->SetScale(Vector3(0.05f, 0.05f, 0.05f), false);
	this->zType = ACTOR_TYPE_PROJECTILE;
	this->zCollisionRadius = 0.3f;
	CalculateCollisionPoints();
}

ProjectileActor::~ProjectileActor()
{

}

