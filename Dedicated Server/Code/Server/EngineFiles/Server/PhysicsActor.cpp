#include "PhysicsActor.h"


PhysicsActor::PhysicsActor() : Actor()
{
	this->zPhysicObj = NULL;
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj, const Vector4& rot) : Actor()
{
	this->zPhysicObj = pObj;
	this->zPhysicObj->SetPosition(startpos);
	this->zPhysicObj->SetQuaternion(rot);
}

PhysicsActor::PhysicsActor( const Vector3& startpos, PhysicsObject* pObj ) : Actor()
{
	this->zPhysicObj = pObj;
	this->zPhysicObj->SetPosition(startpos);
}

PhysicsActor::~PhysicsActor()
{

}
