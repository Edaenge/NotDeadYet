#include "SupplyDropBehavior.h"
#include "SupplyActor.h"
#include "World/World.h"

static const Vector3 GRAVITY = Vector3(.0f, -9.82f, .0f);
//Heights
static const float MAX_AIRBORN_HEIGHT = 200.0f;
static const float MAX_FALL_HEIGHT = 15.0f;

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
	zVelocity.Normalize();
	//Calculate new Position
	zVelocity += GRAVITY * dt;
	newPos += zVelocity;
	
	if( sActor->HasParachute() )
	{
		newParachutePos = sActor->GetParachute()->GetPosition();
		newParachutePos += zVelocity;
		sActor->GetParachute()->SetPosition(newParachutePos);
		
		if( newPos.y <= MAX_FALL_HEIGHT )
		{
			//Notify?
			Actor* parachute = sActor->DetatchParachute();
		}
	}
	else
	{
		//Do Something
	}

	//**Check if the actor has hit the destination**
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