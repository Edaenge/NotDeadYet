#include "ProjectileBehavior.h"


ProjectileBehavior::ProjectileBehavior( Actor* actor, World* world ) : Behavior(actor, world)
{
	this->zVelocity = Vector3(0, 0, 0);
	this->zDamping = 0.99f;
	this->zMoving = true;
}

bool ProjectileBehavior::Update( float dt )
{
	if(!zMoving)
		return false;

	Vector3 newPos;
	
	// Update linear position.
	newPos += (zVelocity* dt);

	//Rotate Mesh
	Vector3 ProjectileDirection = Vector3(0,0,-1);
	Vector3 CameraDirection = this->zVelocity;

	ProjectileDirection.Normalize();
	CameraDirection.Normalize();

	Vector3 around = ProjectileDirection.GetCrossProduct(CameraDirection);
	float angle = acos(ProjectileDirection.GetDotProduct(CameraDirection));

	//Set Values
	this->zActor->SetPosition(newPos);
	this->zActor->SetRotation(Vector4(0,0,0,1));
	this->zActor->SetRotation(around, angle);

	//**Check if the projectile has hit the ground**
	float yValue;
	yValue = this->zWorld->CalcHeightAtWorldPos(newPos.GetXY());

	// If true, stop the projectile and return.
	if(newPos.y <= yValue )
	{
		newPos.y = yValue;
		this->zActor->SetPosition(newPos);
		this->zMoving = false;
		
		return true;
	}

	//**Update Velocity for next update**

	// Update linear velocity from the acceleration.
	this->zVelocity += (GRAVITY * dt);

	// Impose drag.
	this->zVelocity *= pow(zDamping, dt);

	return true;
}