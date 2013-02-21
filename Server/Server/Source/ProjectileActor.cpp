#include "ProjectileActor.h"


ProjectileActor::ProjectileActor(Actor* owner, PhysicsObject* pObj) :
	zOwner(owner), Actor()
{
	this->SetPhysicsObject(pObj);	
}

ProjectileActor::~ProjectileActor()
{

}