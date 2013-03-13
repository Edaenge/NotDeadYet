#include <World/World.h>
#include "ProjectileArrowBehavior.h"
#include "Actor.h"
#include "BioActor.h"
#include "ProjectileActor.h"
#include "WorldActor.h"
#include "ActorSynchronizer.h"

static const Vector3 GRAVITY = Vector3(0, -9.82f, 0);

ProjectileArrowBehavior::ProjectileArrowBehavior( Actor* actor, World* world, float speed ) : Behavior(actor, world)
{
	this->zSpeed = speed;
	this->zVelocity = actor->GetDir();
	this->zDamping = 0.99f;
	this->zMoving = true;
	//this->zLength = 16.396855f;

	PhysicsObject* pObj = actor->GetPhysicsObject();
	if( pObj )
	{
		Vector3 center = pObj->GetBoundingSphere().center;
		center = pObj->GetWorldMatrix() * center;
		this->zLength = ( ( center - actor->GetPosition() ) * 2).GetLength();
	}

	this->zNearByRadius = 370.0f;
}

ProjectileArrowBehavior::~ProjectileArrowBehavior()
{

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
	this->zActor->SetPosition(newPos, false);
	this->zActor->SetRotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	this->zActor->SetRotation(around, angle);
	this->zActor->SetDir(newDir);

	/**Check If the arrow is outside the world.**/
	if( !this->zWorld->IsInside(newPos.GetXZ()) )
	{
		Stop();
		this->zActor->SetPosition(newPos);

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
		this->zActor->SetPosition(newPos);

		return true;
	}

	// If true, stop the projectile and return.
	float middle = zLength * 0.5f;
	float yTip = newPos.y - middle;
	if(yTip <= yValue )
	{
 		middle += yValue;
 		newPos.y = middle;

		this->Stop();
		this->zActor->SetPosition(newPos);
		
		return true;
	}

	//**Update Velocity for next update**

	// Update linear velocity from the acceleration.
	this->zVelocity += (GRAVITY * dt);

	// Impose drag.
	this->zVelocity *= pow(zDamping, dt);
	
	//Update-Notify Position
	this->zActor->SetPosition(newPos);

	/***Check collisions***/

	//Check if the arrow has hit a dynamic actor
	Actor* collide = RayVsMeshDynamicActorCollision(zLength, newPos);

	if(collide)
	{
		this->Stop();
		ProjectileArrowCollide PAC;
		PAC.zActor = collide;
		NotifyObservers(&PAC);

		if( BioActor* bioActor = dynamic_cast<BioActor*>(collide) )
		{
			if( bioActor->IsAlive() )
			{
				ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(this->zActor);
				bioActor->TakeDamage( projActor->GetDamage(), projActor->GetOwner() );
			}
		}

		return true;
	}

	//Check if the arrow has hit a static actor
	collide = RayVsMeshStaticActorCollision(zLength, newPos);

	if(collide)
	{
		this->Stop();
		ProjectileArrowCollide PAC;
		PAC.zActor = collide;
		NotifyObservers(&PAC);

		return true;
	}

	return false;
}

Actor* ProjectileArrowBehavior::RayVsMeshCollision( const Actor* actor, const Vector3& rayOrigin, float& range, const std::set<Actor*>& actors )
{
	if( !actor || !actor->CanCollide() )
		return NULL;

	float rangeWithin = 1.0f + range;

	PhysicsCollisionData data;

	ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(this->zActor); 
	
	if( !projActor )
		return Behavior::RayVsMeshCollision(actor, rayOrigin, range, actors);

	Actor* collide = NULL;
	Actor* owner = NULL;
	Vector3 pos = actor->GetPosition();
	Vector3 dir = actor->GetDir();

	owner = projActor->GetOwner();

	if(!owner)
		return NULL;
	
	auto it_end = actors.end();
	for (auto it = actors.begin(); it != it_end; it++)
	{
		if( *it == this->zActor )
			continue;
		if( *it == owner )
			continue;
		if( !(*it)->CanCollide() )
			continue;
		
		float distance = ( pos.GetXZ() - (*it)->GetPosition().GetXZ() ).GetLength();

		if( distance > rangeWithin )
			continue;

		PhysicsObject* targetObject = (*it)->GetPhysicsObject();
		data = GetPhysics()->GetCollisionRayMesh(rayOrigin, dir, targetObject);

		if(data.collision && data.distance < range)
		{
			range = data.distance;
			collide = (*it);
		}

	}

	return collide;
}