#include <World/World.h>
#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"
#include "BioActor.h"

PlayerBehavior::PlayerBehavior(Actor* actor, World* world, Player* player) : 
	Behavior(actor, world)
{	
	this->zPlayer = player;
	this->zVelocity = Vector3(0.0f, 0.0f, 0.0f);
	this->zVelDown = 0.0f;

	this->zNearByRadius = 100.0f;

	// Bigger Anchor For Players
	zAnchor->radius = 50.0f;

	this->zLenght = 0.0f;

	PhysicsObject* pObj = this->zActor->GetPhysicsObject();

	if( pObj != NULL)
	{
		Vector3 center = pObj->GetBoundingSphere().center;
		center = pObj->GetWorldMatrix() * center;
		zLenght = ( ( center - actor->GetPosition() ) * 2).GetLength();
	}
}

PlayerBehavior::~PlayerBehavior()
{
	this->zNearActors.clear();
	this->zNearDynamicActors.clear();
	this->zNearStaticActors.clear();
}

bool PlayerBehavior::ProcessClientData( const Vector3& direction, const Vector4& rotation )
{
	this->zActor->SetDir(direction);
	this->zActor->SetRotation(rotation);

	return true;
}

bool PlayerBehavior::Update(float dt)
{
	static const float energy_Coeff = GetPlayerConfig()->GetVariable(ENERGY_COEFF);
	float energy = zActor->GetEnergy();

	if (energy < 200.0f)

		energy += energy_Coeff * dt;

	if (energy >= 200.0f)
		energy = 200.0f;

	zActor->SetEnergy(energy);

	return false;
}

Actor* PlayerBehavior::HandleCollision()
{
	Vector3 pActor_position = zActor->GetPosition();
	Vector3 pActor_rewind_dir;
	Actor* collide = NULL;

	/* Check Collisions against Dynamic Actors */
	collide = DistanceDynamicActorCollision();

	if( collide )
	{
		pActor_rewind_dir = ( collide->GetPosition() - pActor_position );
		pActor_rewind_dir.Normalize();
		Vector3 target_rewind_dir = pActor_rewind_dir * -1;

		//If it's an BioActor
		if (BioActor* bioActor = dynamic_cast<BioActor*>(collide) )
		{
			if( bioActor->IsAlive() )
			{
				if( bioActor->HasMoved() )
					bioActor->SetPosition( bioActor->GetPosition() - (target_rewind_dir * 0.1f) );

				zActor->SetPosition( pActor_position - (pActor_rewind_dir * 0.1f) );
			}
		}
		else
		{
			zActor->SetPosition( pActor_position - (pActor_rewind_dir * 0.1f) );
		}

	}
	else
	{
		/* Check Collisions against Static Actors */
		collide = DistanceStaticActorCollision();

		if( collide )
		{
			pActor_rewind_dir = (collide->GetPosition() - pActor_position);
			pActor_rewind_dir.Normalize();

			zActor->SetPosition( pActor_position - (pActor_rewind_dir * 0.1f) );
		}

	}

	return collide;
}

bool PlayerBehavior::HandleWaterCollision()
{
	Vector3 pActor_position = zActor->GetPosition();
	Vector3 pActor_rewind_dir;

	/* Check if Actor is under the Water*/
	if( zWorld->GetWaterDepthAt( pActor_position.GetXZ() ) >  zLenght * 0.5f )
	{
		pActor_rewind_dir = zVelocity;
		pActor_rewind_dir.Normalize();
		zActor->SetPosition( pActor_position - (pActor_rewind_dir * 0.1f), false );

		return true;
	}

	return false;
}
