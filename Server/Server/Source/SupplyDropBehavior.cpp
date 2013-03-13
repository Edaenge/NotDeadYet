#include "SupplyDropBehavior.h"
#include "SupplyActor.h"
#include "World/World.h"
#include "ActorEvent.h"
#include "SoundHandler.h"


static const Vector3 GRAVITY = Vector3(.0f, -9.82f, .0f);
//Heights
static const float MAX_AIRBORN_HEIGHT = 200.0f;
static const float MAX_FALL_HEIGHT = 20.0f;
//MAX y velocity
static const float MAX_FALL_VELOCITY_PARACHUTE			= 10.0f;
static const float MAX_FALL_VELOCITY_NONE_PARACHUTE		= 45.0f;

SupplyDropBehavior::SupplyDropBehavior( Actor* actor, World* world, Vector2& destination, SoundHandler* soundHandler )
	: Behavior(actor, world), zSoundHandler(soundHandler)
{
	AddObserver(this->zSoundHandler);

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
		if( sActor->IsParachuteAttached() )
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
	RemoveObserver(this->zSoundHandler);
}

bool SupplyDropBehavior::Update( float dt )
{
	if( !this->zMoving )
		return true;

	SupplyActor* sActor = dynamic_cast<SupplyActor*>(zActor);

	if( !sActor )
		return true;

	Actor* parachute = sActor->GetParachute();
	Vector3 newPos;
	Vector3 newParachutePos;
	static bool crateLanded = false;

	//Get Positions
	newPos = sActor->GetPosition();
	if( parachute )
		newParachutePos = parachute->GetPosition();

	//Calculate new Position
	if( sActor->IsParachuteAttached() )
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
		
		//Update crate pos
		newPos += zVelocity * dt;
		sActor->SetPosition(newPos);

		//Update parachute position
		newParachutePos += zVelocity * dt;
		parachute->SetPosition(newParachutePos);

		//Detach within this height
		if( newPos.y <= MAX_FALL_HEIGHT )
		{
			sActor->DetatchParachute();
		}

	}
	//No Parachute Attached
	else
	{
		if( !crateLanded )
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

			//Update crate pos
			newPos += zVelocity * dt;
			this->zActor->SetPosition(newPos);
		}

		//Parachute
		if( parachute )
		{
			Vector3 vel = zVelocity;
			vel.y = MAX_FALL_VELOCITY_PARACHUTE * 0.5f;
			
			newParachutePos += (vel * -1) * dt;
			parachute->SetPosition(newParachutePos);
		}

	}

	/***Check if the crate has hit the destination***/
	if( !crateLanded && newPos.y <= zDestination.y )
	{
		sActor->SetPosition(zDestination);

		if( !parachute )
		{
			this->zMoving = false;
			return true;
		}

		crateLanded = true;

		//Notify Observers
		SupplyDropLanded e;
		e.zActor = sActor;
		NotifyObservers(&e);
		
	}

	/***Check if the parachute has hit the destination***/
	if( newParachutePos.y <= zDestination.y )
	{
		parachute->SetScale( Vector3(0,0,0) );
		zMoving = false;

		return true;
	}

	return false;
}