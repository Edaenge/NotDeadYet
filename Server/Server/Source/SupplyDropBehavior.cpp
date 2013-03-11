#include "SupplyDropBehavior.h"
#include "SupplyActor.h"
#include "World/World.h"

static const Vector3 GRAVITY = Vector3(.0f, -9.82f, .0f);

SupplyDropBehavior::SupplyDropBehavior( Actor* actor, World* world, Vector2& destination )
	: Behavior(actor, world)
{
	this->zMoving = true;
	this->zSpeed = 3.0f;
	
	if(!world->IsInside( destination) )
		destination = this->zWorld->GetWorldCenter();

	float yValue = world->CalcHeightAtWorldPos(destination);
	this->zDestination = Vector3(destination.x, yValue, destination.y);
	
	this->zVelocity = this->zActor->GetPosition() - this->zDestination;
}

SupplyDropBehavior::~SupplyDropBehavior()
{

}

bool SupplyDropBehavior::Update( float dt )
{
	if( !zMoving )
		return true;

	SupplyActor* sActor = dynamic_cast<SupplyActor*>(zActor);

	if( !sActor )
		return true;

	Vector3 newPos;

	if( !sActor->HasParachute() )
	{
		//Calculate new Position
		newPos = sActor->GetPosition();
		zVelocity.Normalize();
		zVelocity *= zSpeed;
		newPos += (zVelocity * dt);
	}
	else
	{
		//Do Something
	}

	//**Check if the actor has hit the destination**
	if( newPos.y <= zDestination.y )
	{
		this->zMoving = false;
		sActor->SetPosition(newPos);

		return true;
	}

	//**Update Velocity for next update**

	// Update linear velocity from the acceleration.
	this->zVelocity += (GRAVITY * dt);

	// Impose drag.
	//this->zVelocity *= pow(zDamping, dt);
	
	//Update-Notify Position
	this->zActor->SetPosition(newPos);


	return false;
}

