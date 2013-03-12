#include "SupplyDropBehavior.h"
#include "SupplyActor.h"
#include "World/World.h"

static const Vector3 GRAVITY = Vector3(.0f, -9.82f, .0f);
//Heights
static const float MAX_AIRBORN_HEIGHT = 200.0f;
static const float MAX_FALL_HEIGHT = 15.0f;
//MAX y velocity
static const float MAX_FALL_VELOCITY_PARACHUTE			= 10.0f;
static const float MAX_FALL_VELOCITY_NONE_PARACHUTE		= 45.0f;

SupplyDropBehavior::SupplyDropBehavior( Actor* actor, World* world, Vector2& destination )
	: Behavior(actor, world)
{
	this->zMoving = true;
	this->zSpeed = -10.0f;
	
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
		if( zVelocity.y > MAX_FALL_VELOCITY_PARACHUTE)
			zVelocity.y = MAX_FALL_VELOCITY_PARACHUTE;
		
		//If true, constant velocity hasn't been reached
		if( zVelocity.y != MAX_FALL_VELOCITY_PARACHUTE )
		{
			Vector3 normalizedVec = zVelocity;
			normalizedVec.Normalize();
			zVelocity = normalizedVec + GRAVITY * dt;
		}
		
		//Update position
		newPos += zVelocity;

		//Update parachute position
		newParachutePos = sActor->GetParachute()->GetPosition();
		newParachutePos += zVelocity;
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
		//Do stuff here
	}

	/***Check if the actor has hit the destination***/
	if( newPos.y <= zDestination.y )
	{
		this->zMoving = false;
		sActor->SetPosition(zDestination);

		return true;
	}


	//Update-Notify Position
	this->zActor->SetPosition(newPos);
	MaloW::Debug("SupplyDrop Y Position: " + MaloW::convertNrToString(newPos.y));


	return false;
}