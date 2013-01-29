#include "PhysicsActor.h"


PhysicsActor::PhysicsActor() : Actor()
{
	this->zPhysicObj = NULL;
	this->zScaleChanged = false;
	this->zInitialDirection = Vector3(0,0,-1);
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj, const Vector4& rot) : Actor()
{
	this->zPhysicObj = pObj;
	this->zScaleChanged = false;
	this->zPhysicObj->SetPosition(startpos);
	this->zPhysicObj->SetQuaternion(rot);
	this->zInitialDirection = Vector3(0,0,-1);
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj) : Actor()
{
	this->zPhysicObj = pObj;
	this->zScaleChanged = false;
	this->zPhysicObj->SetPosition(startpos);
	this->zInitialDirection = Vector3(0,0,-1);
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