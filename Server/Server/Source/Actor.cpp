#include "Actor.h"

static unsigned int NextActorID = 0;

Actor::Actor() :
	zID(++NextActorID),
	zPhysicsObject(NULL)
{
	this->zEnergy = 0.0f;
	SetScale(Vector3(0.05f, 0.05f, 0.05f), false);
}

Actor::~Actor()
{
	ActorDeleteEvent ADE;
	ADE.zActor = this;
	NotifyObservers(&ADE);
}

void Actor::SetPosition( const Vector3& pos, const bool notify /*= true*/ )
{
	this->zPreviousPos = this->zPos;
	this->zPos = pos;
	
	if (this->zPhysicsObject)
	{
		this->zPhysicsObject->SetPosition(pos);
	}

	if(notify)
	{
		ActorPositionEvent APE;
		APE.zActor = this;
		NotifyObservers(&APE);
	}

}

void Actor::SetRotation( const Vector4& rot, const bool notify /*= true*/ )
{
	zRot = rot;
	if (this->zPhysicsObject)
	{
		this->zPhysicsObject->SetQuaternion(rot);
	}

	if(notify)
	{
		ActorRotationEvent ARE;
		ARE.zActor = this;
		NotifyObservers(&ARE);
	}

}

void Actor::SetRotation( const Vector3& around, const float angle, const bool notify /*= true*/ )
{
	if(!this->zPhysicsObject) return;
	
	this->zPhysicsObject->RotateAxis(around, angle);
	this->zRot = this->zPhysicsObject->GetRotationQuaternion();
	
	if(notify)
	{
		ActorRotationEvent ARE;
		ARE.zActor = this;
		NotifyObservers(&ARE);
	}
}

void Actor::SetScale( const Vector3& scale, const bool notify /*= true*/ )
{
	zScale = scale;
	if (this->zPhysicsObject)
	{
		this->zPhysicsObject->SetScaling(scale);
	}

	if(notify)
	{
		ActorScaleEvent ASE;
		ASE.zActor = this;
		NotifyObservers(&ASE);
	}
}

void Actor::SetDir( const Vector3& dir, const bool notify /*= true*/ )
{
	zDir = dir;
	if(notify)
	{
		ActorScaleEvent ASE;
		ASE.zActor = this;
		NotifyObservers(&ASE);
	}
}

void Actor::SetPhysicsObject( PhysicsObject* object )
{
	zPhysicsObject = object;
}

void Actor::SetEnergy( float energy, const bool notify /*= true*/ )
{
	if( energy != this->zEnergy)
	{
		this->zEnergy = energy;

		if(notify)
		{
			ActorPhysicalConditionEnergyEvent e;
			e.zActor = this;
			NotifyObservers(&e);
		}
	}
}

const std::string Actor::GetModel() const
{
	if (zPhysicsObject)
		return zPhysicsObject->GetModel();


	return this->zModel;
}

void Actor::SetModel( const std::string& model )
{
	this->zModel = model;

	if (zPhysicsObject)
		this->zPhysicsObject->SetModel(model);
}

bool Actor::CanCollide() const
{
	if( zPhysicsObject )
		return true;

	return false;
}