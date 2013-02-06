#include "Actor.h"

long Actor::zNextAID = 500;

void Actor::SetPosition( const Vector3& pos )
{
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