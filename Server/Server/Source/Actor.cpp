#include "Actor.h"

static unsigned int NextActorID = 0;

Actor::Actor() :
	zID(++NextActorID),
	zPhysicsObject(NULL)
{
	this->zEnergy = 0.0f;
	this->zCollisionRadius = 0.3f;
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
		ActorDirEvent ASE;
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

void Actor::CalculateCollisionPoints()
{
	// 	Vector3 centerPos;
	// 	float humanRadius = 0.5f;//m
	// 	float distanceInAxes;
	// 	//Pythagoras theorem x2 = y2 + z2
	// 	//both distances are the same, therefore x2 = y2 + y2 = 2y2
	// 	//(distanceInAxes = y)
	float distanceInAxes = sqrtf(powf(zCollisionRadius, 2.0f) * 0.5f); //use * 0.5f instead.
	// 	//Build vectors going clockwise
	// 
	Vector2 one = Vector2(distanceInAxes, distanceInAxes);//+X+Z
	Vector2 two = Vector2(distanceInAxes, -distanceInAxes);//+X-Z
	Vector2 three = Vector2(-distanceInAxes, -distanceInAxes);//-X-Z
	Vector2 four = Vector2(-distanceInAxes, distanceInAxes);//-X+Z

	zCollisionPoints[0] = one;
	zCollisionPoints[1] = two;
	zCollisionPoints[2] = three;
	zCollisionPoints[3] = four;
}