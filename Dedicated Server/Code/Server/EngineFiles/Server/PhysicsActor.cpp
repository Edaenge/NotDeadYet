#include "PhysicsActor.h"


PhysicsActor::PhysicsActor() : Actor()
{
	this->zPhysicObj = NULL;
	this->zScaleChanged = false;
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj, const Vector4& rot) : Actor()
{
	this->zPhysicObj = pObj;
	this->zScaleChanged = false;
	this->zPhysicObj->SetPosition(startpos);
	this->zPhysicObj->SetQuaternion(rot);
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj) : Actor()
{
	this->zPhysicObj = pObj;
	this->zScaleChanged = false;
	this->zPhysicObj->SetPosition(startpos);
}

PhysicsActor::~PhysicsActor()
{

}

void PhysicsActor::AddChangedData(string& mess, NetworkMessageConverter* nmc)
{
	if (this->zScaleChanged)
	{
		float scale = this->GetScale().x;
		mess += nmc->Convert(MESSAGE_TYPE_SCALE, scale);
		this->zScaleChanged = false;
	}
}