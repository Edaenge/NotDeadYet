#include "Actor.h"

static unsigned int NextActorID = 0;

Actor::Actor() :
	zID(++NextActorID),
	zPhysicsObject(0)
{
	this->zPreviousPos = this->zPos;
	SetScale(Vector3(0.05f, 0.05f, 0.05f));
}

Actor::~Actor()
{
	ActorDeleteEvent ADE;
	ADE.zActor = this;
	NotifyObservers(&ADE);
}

void Actor::RewindPosition()
{
	Vector3 temp = this->zPreviousPos;
	SetPosition(this->zPreviousPos);
	this->zPreviousPos = temp;
}

void Actor::SetPosition( const Vector3& pos )
{
	this->zPreviousPos = this->zPos;
	this->zPos = pos;
	if (this->zPhysicsObject)
	{
		this->zPhysicsObject->SetPosition(pos);
	}

	ActorPositionEvent APE;
	APE.zActor = this;
	NotifyObservers(&APE);
}

void Actor::SetRotation( const Vector4& rot )
{
	zRot = rot;
	if (this->zPhysicsObject)
	{
		this->zPhysicsObject->SetQuaternion(rot);
	}

	ActorRotationEvent ARE;
	ARE.zActor = this;
	NotifyObservers(&ARE);
}

void Actor::SetRotation( const Vector3& around, const float angle )
{
	if(!this->zPhysicsObject) return;
	
	this->zPhysicsObject->RotateAxis(around, angle);
	this->zRot = this->zPhysicsObject->GetRotationQuaternion();

	ActorRotationEvent ARE;
	ARE.zActor = this;
	NotifyObservers(&ARE);
}

void Actor::SetScale( const Vector3& scale )
{
	zScale = scale;
	if (this->zPhysicsObject)
	{
		this->zPhysicsObject->SetScaling(scale);
	}

	ActorScaleEvent ASE;
	ASE.zActor = this;
	NotifyObservers(&ASE);
}

void Actor::SetDir( const Vector3& dir )
{
	zDir = dir;

	ActorScaleEvent ASE;
	ASE.zActor = this;
	NotifyObservers(&ASE);
}

void Actor::SetPhysicsObject( PhysicsObject* object )
{
	zPhysicsObject = object;
}

const std::string& Actor::GetModel() const
{
	if ( zPhysicsObject )
		return zPhysicsObject->GetModel();

	static const std::string none = "Media/Models/Ball.obj";
	return none;
}
