#include "SupplyDropBehavior.h"
#include "SupplyActor.h"
#include "World/World.h"

static const Vector3 GRAVITY = Vector3(.0f, -9.82f, .0f);
//Heights
static const float MAX_AIRBORN_HEIGHT = 200.0f;
static const float MAX_FALL_HEIGHT = 15.0f;
//MAX y velocity
static const float MAX_FALL_VELOCITY_PARACHUTE			= 15.0f;
static const float MAX_FALL_VELOCITY_NONE_PARACHUTE		= 45.0f;

SupplyDropBehavior::SupplyDropBehavior( Actor* actor, World* world, Vector2& destination )
	: Behavior(actor, world)
{
	this->zMoving = true;

	if(!world->IsInside( destination) )
		destination = this->zWorld->GetWorldCenter();

	float yValue = world->CalcHeightAtWorldPos(destination);
	this->zDestination = Vector3(destination.x, yValue, destination.y);
	
	this->zVelocity = this->zDestination - this->zActor->GetPosition();
	this->zVelocity.Normalize();

	SupplyActor* sActor = dynamic_cast<SupplyActor*>(zActor);

	if( sActor )
	{
		Vector3 pos = sActor->GetPosition();

		//Check Heights
		if( sActor->HasParachute() )
		{
			if( pos.y > MAX_AIRBORN_HEIGHT )
			{
				pos.y = MAX_AIRBORN_HEIGHT;
				sActor->SetPosition(pos, false);
			}
			else if( pos.y < MAX_FALL_HEIGHT )
			{
				pos.y = MAX_FALL_HEIGHT;
				sActor->SetPosition(pos);
			}
		}
		else
		{
			if( pos.y > MAX_FALL_HEIGHT )
			{
				pos.y = MAX_FALL_HEIGHT;
				sActor->SetPosition(pos, false);
			}
			else if( pos.y < 0 )
			{
				pos.y = MAX_FALL_HEIGHT;
				sActor->SetPosition(pos);
			}
		}

	}

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
	Vector3 newParachutePos;
	newPos = sActor->GetPosition();

	//Calculate new Position
	if( sActor->HasParachute() )
	{
		//If velocity fall is bigger than max, constant velocity has been reached
		if( fabs(zVelocity.y) > MAX_FALL_VELOCITY_PARACHUTE )
		{	
			zVelocity.y = MAX_FALL_VELOCITY_PARACHUTE * -1;
		}
		
		//If true, constant velocity hasn't been reached
		if( fabs(zVelocity.y) != MAX_FALL_VELOCITY_PARACHUTE )
		{
			zVelocity = zVelocity + GRAVITY * dt;
		}
		
		//Update parachute position
		newParachutePos = sActor->GetParachute()->GetPosition();
		newParachutePos += zVelocity * dt;
		sActor->GetParachute()->SetPosition(newParachutePos);
		
		//Detach within this height
		if( newPos.y <= MAX_FALL_HEIGHT )
		{
			//Notify?
			Actor* parachute = sActor->DetatchParachute();
		}
	}
	//No Parachute
	else
	{
		//If velocity fall is bigger than max, constant velocity has been reached
		if( fabs(zVelocity.y) > MAX_FALL_VELOCITY_NONE_PARACHUTE )
		{	
			zVelocity.y = MAX_FALL_VELOCITY_NONE_PARACHUTE * -1;
		}

		//If true, constant velocity hasn't been reached
		if( fabs(zVelocity.y) != MAX_FALL_VELOCITY_NONE_PARACHUTE )
		{
			zVelocity = zVelocity + GRAVITY * dt;
		}

	}

	//Update position
	newPos += zVelocity * dt;

	/***Check if the actor has hit the destination***/
	if( newPos.y <= zDestination.y )
	{
		this->zMoving = false;
		sActor->SetPosition(zDestination);

		return true;
	}


	//Update-Notify Position
	this->zActor->SetPosition(newPos);

	return false;
}