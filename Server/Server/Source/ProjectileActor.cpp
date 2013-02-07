#include "ProjectileActor.h"


ProjectileActor::ProjectileActor(Actor* owner, PhysicsObject* pObj) :
	zOwner(owner), Actor()
{
	this->SetPhysicsObject(pObj);	
	this->SetDir(owner->GetDir());
	this->SetPosition(owner->GetPosition());
}

ProjectileActor::~ProjectileActor()
{

}