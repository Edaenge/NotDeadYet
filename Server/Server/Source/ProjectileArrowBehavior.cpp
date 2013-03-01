#include <World/World.h>
#include "ProjectileArrowBehavior.h"
#include "Actor.h"

static const Vector3 GRAVITY = Vector3(0, -9.82f, 0);

ProjectileArrowBehavior::ProjectileArrowBehavior( Actor* actor, World* world ) : Behavior(actor, world)
{
	this->zSpeed = 30.0f;
	this->zVelocity = actor->GetDir();
	this->zDamping = 0.01f;
	this->zMoving = true;
	//this->zLength = 16.396855f;

	PhysicsObject* actorPhys = actor->GetPhysicsObject();
	Vector3 center = actorPhys->GetBoundingSphere().center;
	center = actorPhys->GetWorldMatrix() * center;

	this->zLength = ( ( center - actor->GetPosition() ) * 2).GetLength();

}

bool ProjectileArrowBehavior::Update( float dt )
{
	if(!zMoving)
		return true;

	Vector3 newPos;
	Vector3 newDir;
	
	// Update linear position.
	newPos =  this->zActor->GetPosition();
	zVelocity.Normalize();
	zVelocity *= zSpeed;
	newPos += (zVelocity * dt);
	newDir = zVelocity;
	newDir.Normalize();

	//Rotate Mesh
	Vector3 ProjectileStartDirection = Vector3(0,0,-1);
	Vector3 ProjectileMoveDirection = newDir;

	ProjectileStartDirection.Normalize();

	Vector3 around = ProjectileStartDirection.GetCrossProduct(ProjectileMoveDirection);
	float angle = acos(ProjectileStartDirection.GetDotProduct(ProjectileMoveDirection));

	//Set Values
	this->zActor->SetPosition(newPos);
	this->zActor->SetRotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	this->zActor->SetRotation(around, angle);
	this->zActor->SetDir(newDir);

	/**Check If the arrow is outside the world.**/
	if( !this->zWorld->IsInside(newPos.GetXZ()) )
	{
		Stop();
		return true;
	}
	//**Check if the projectile has hit the ground**
	float yValue = std::numeric_limits<float>::lowest();
	try
	{
		yValue = this->zWorld->CalcHeightAtWorldPos(newPos.GetXZ());
	}
	catch(...)
	{
		Stop();
		return true;
	}

	// If true, stop the projectile and return.
	Vector3 scale = this->zActor->GetScale();
	float middle = zLength * 0.5f;
	float yTip = newPos.y - middle;
	if(yTip <= yValue )
	{
 		middle += yValue;
 		newPos.y = middle;

		this->zActor->SetPosition(newPos);
		this->zMoving = false;
		
		return true;
	}

	//**Update Velocity for next update**

	// Update linear velocity from the acceleration.
	this->zVelocity += (GRAVITY * dt);

	// Impose drag.
	this->zVelocity *= pow(zDamping, dt);

	return false;
}