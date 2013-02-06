#include "ProjectileActor.h"

ProjectileActor::ProjectileActor(Actor* shooter, PhysicsObject* pObj) :
	zShooter(shooter)
{
	this->zPhysicsObject = pObj;
}

ProjectileActor::~ProjectileActor()
{

}