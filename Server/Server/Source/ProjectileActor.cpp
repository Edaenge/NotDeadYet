#include "ProjectileActor.h"


ProjectileActor::ProjectileActor(Actor* owner, PhysicsObject* pObj) :
	zOwner(owner), Actor()
{
	this->SetPhysicsObject(pObj);	
	this->SetDir(owner->GetDir());

	Vector3 pos = owner->GetPosition();
	pos.y += 1.75f;
	this->SetPosition(pos);
}

ProjectileActor::~ProjectileActor()
{

}