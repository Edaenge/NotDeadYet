#include "PhysicsActor.h"


PhysicsActor::PhysicsActor() : Actor()
{
	this->zPhysicObj = NULL;
	this->zScaleChanged = true;
	this->zInitialDirection = Vector3(0,0,-1);
	this->zModelLength = 0.1f;
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj, const Vector4& rot) : Actor()
{
	this->zPhysicObj = pObj;
	this->zScaleChanged = true;
	this->zPhysicObj->SetPosition(startpos);
	this->zPhysicObj->SetQuaternion(rot);
	this->zInitialDirection = Vector3(0,0,-1);
	this->zModelLength = 0.1f;
}

PhysicsActor::PhysicsActor(const Vector3& startpos, PhysicsObject* pObj) : Actor()
{
	this->zPhysicObj = pObj;
	this->zScaleChanged = true;
	this->zPhysicObj->SetPosition(startpos);
	this->zInitialDirection = Vector3(0,0,-1);
	this->zModelLength = 0.1f;
}

PhysicsActor::~PhysicsActor()
{

}

std::string PhysicsActor::ToMessageString( NetworkMessageConverter* NMC )
{
	if(!this->zPhysicObj)
		return "";

	std::string msg;
	Vector3 pos = this->zPhysicObj->GetPosition();
	Vector4 rot = this->zPhysicObj->GetRotationQuaternion();

	msg = NMC->Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += NMC->Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

	if (this->zScaleChanged)
	{
		Vector3 scale = this->GetScale();
		msg += NMC->Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		this->zScaleChanged = false;
	}
	
	return msg;
}