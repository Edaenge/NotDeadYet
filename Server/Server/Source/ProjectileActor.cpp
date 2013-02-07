#include "ProjectileActor.h"

ProjectileActor::ProjectileActor(Actor* shooter, PhysicsObject* pObj) :
	zShooter(shooter)
{
	this->SetPhysicsObject(pObj);
}

ProjectileActor::~ProjectileActor()
{

}