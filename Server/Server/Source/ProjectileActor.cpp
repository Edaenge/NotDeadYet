#include "ProjectileActor.h"

ProjectileActor::ProjectileActor(Actor* owner, PhysicsObject* pObj) :
	zOwner(owner), Actor()
{
	this->zPhysicsObject = pObj;	
	this->zDir = owner->GetDir();
	this->zPos = owner->GetPosition();
}

ProjectileActor::~ProjectileActor()
{

}