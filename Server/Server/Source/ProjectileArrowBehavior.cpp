#include <World/World.h>
#include "ProjectileArrowBehavior.h"
#include "Actor.h"
#include "BioActor.h"
#include "ProjectileActor.h"
#include "Physics.h"
#include "WorldActor.h"
#include "ActorSynchronizer.h"

static const Vector3 GRAVITY = Vector3(0, -9.82f, 0);

ProjectileArrowBehavior::ProjectileArrowBehavior( Actor* actor, World* world ) : Behavior(actor, world)
{
	this->zSpeed = 30.0f;
	this->zVelocity = actor->GetDir();
	this->zDamping = 0.01f;
	this->zMoving = true;
	//this->zLength = 16.396855f;

	PhysicsObject* pObj = actor->GetPhysicsObject();
	if( pObj )
	{
		Vector3 center = pObj->GetBoundingSphere().center;
		center = pObj->GetWorldMatrix() * center;
		this->zLength = ( ( center - actor->GetPosition() ) * 2).GetLength();
	}

	this->zNearActorsIndex = 0;
	this->zNearByRadius = 370.0f;
}

ProjectileArrowBehavior::~ProjectileArrowBehavior()
{
	this->zNearActors.clear();
	this->zNearBioActors.clear();
	this->zNearWorldActors.clear();
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

	//Check if the arrow has hit a BioActor
	Actor* collide = CheckBioActorCollision();

	if(collide)
	{
		this->Stop();
		ProjectileArrowCollide PAC;
		PAC.zActor = collide;
		NotifyObservers(&PAC);

		BioActor* bioActor = dynamic_cast<BioActor*>(collide);

		if( bioActor->IsAlive() )
		{
			ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(this->zActor);
			bioActor->TakeDamage( projActor->GetDamage(), projActor->GetOwner() );
		}

		return true;
	}

	//Check if the arrow has hit a WorldActor
	collide = CheckWorldActorCollision();

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

void ProjectileArrowBehavior::SetNearBioActors( std::set<Actor*> actors )
{
	this->zNearBioActors = actors;
	this->zNearBioActors.erase(this->zActor);
	this->zNearActors.clear();

	auto it_end = this->zNearBioActors.end();
	for (auto it = this->zNearBioActors.begin(); it != it_end; it++)
	{
		this->zNearActors.insert(*it);
	}
}

void ProjectileArrowBehavior::SetNearWorldActors( std::set<Actor*> actors )
{
	this->zNearWorldActors = actors;
	this->zNearWorldActors.erase(this->zActor);
	this->zNearActors.clear();

	auto it_end = this->zNearWorldActors.end();
	for (auto it = this->zNearWorldActors.begin(); it != it_end; it++)
	{
		this->zNearActors.insert(*it);
	}
}

bool ProjectileArrowBehavior::RefreshNearCollideableActors( const std::set<Actor*>& actors )
{
	bool canCollide = this->zActor->CanCollide();

	if(!canCollide)
		return false;

	unsigned int size = actors.size();
	// Increment 5%
	unsigned int increment = (unsigned int)(size * 0.05);

	Vector3 pos = this->zActor->GetPosition();

	if(zNearActorsIndex >= size)
		zNearActorsIndex = 0;

	auto it = actors.begin();
	std::advance(it, zNearActorsIndex);

	//Check if the new actors is within range.
	for (unsigned int i = 0; i < increment; i++)
	{

		if( it == actors.end() )
		{
			zNearActorsIndex = 0;
			it = actors.begin();
		}

		if( (*it) != this->zActor && (*it)->CanCollide() )
		{
			Vector3 vec = (*it)->GetPosition() - pos;
			BioActor* bioActor = dynamic_cast<BioActor*>(*it);
			WorldActor* worldActor = dynamic_cast<WorldActor*>(*it);

			auto found = this->zNearActors.find(*it);

			if( vec.GetLength() <= zNearByRadius )
			{
				if( found == zNearActors.end() )
					zNearActors.insert(*it);

				if(bioActor && zNearBioActors.find(*it) == zNearBioActors.end())
					this->zNearBioActors.insert(*it);

				else if(worldActor && zNearWorldActors.find(*it) == zNearWorldActors.end())
					this->zNearWorldActors.insert(*it);
			}
			else
			{
				if( found != zNearActors.end() )
					zNearActors.erase(*it);

				if( bioActor && zNearBioActors.find(*it) != zNearBioActors.end() )
					this->zNearBioActors.erase(*it);

				else if( worldActor && zNearWorldActors.find(*it) != zNearWorldActors.end() )
					this->zNearWorldActors.erase(*it);
			}
		}

		zNearActorsIndex++;
		it++;
	}

	return true;
}

Actor* ProjectileArrowBehavior::CheckCollision( const Vector3& pos, const float& radius, const std::set<Actor*>& actors )
{
	if( !this->zActor || !this->zActor->CanCollide() )
		return NULL;

	float range = this->zLength;
	float rangeWithin = 1.0f + range;

	PhysicsCollisionData data;
	ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(this->zActor); 
	Actor* collide = NULL;
	Actor* owner = NULL;

	if(projActor)
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
		data = GetPhysics()->GetCollisionRayMesh(pos, this->zActor->GetDir(), targetObject);

		if(data.collision && data.distance < range)
		{
			range = data.distance;
			collide = (*it);
		}

	}

	return collide;
}

Actor* ProjectileArrowBehavior::CheckBioActorCollision()
{
	const Vector3 thisActorPosition = this->zActor->GetPosition();

	if( !this->zActor->CanCollide() )
		return NULL;

	return CheckCollision(thisActorPosition, this->zActor->GetCollisionRadius(), zNearBioActors);
}

Actor* ProjectileArrowBehavior::CheckWorldActorCollision()
{
	if( !this->zActor )
		return NULL;

	if( !this->zActor->CanCollide() )
		return NULL;

	const Vector3 thisActorPosition = this->zActor->GetPosition();
	const Vector2 thisActorPositionXZ = thisActorPosition.GetXZ();

	//Check all collision points
	const Vector2* collidePoints = this->zActor->GetCollisionPoints();
	Vector2 blockedPos;
	bool isBlocking = false;

	for( int i = 0; i < 4 && !isBlocking; i++)
	{
		blockedPos = thisActorPositionXZ + collidePoints[i];
		try
		{
			if( this->zWorld->IsBlockingAt(blockedPos) )
				isBlocking = true;
		}
		catch (...)
		{
			isBlocking = false;
		}
		
	}

	if( !isBlocking )
		return NULL;

	return CheckCollision(thisActorPosition, this->zActor->GetCollisionRadius(), zNearWorldActors);
}

void ProjectileArrowBehavior::OnEvent( Event* e )
{
	Behavior::OnEvent(e);

	if( ActorRemoved* AR = dynamic_cast<ActorRemoved*>(e) )
	{
		auto found = this->zNearActors.find(AR->zActor);
		if(found != this->zNearActors.end())
			this->zNearActors.erase(found);

		found = this->zNearBioActors.find(AR->zActor);
		if( found != this->zNearBioActors.end() )
			this->zNearBioActors.erase(found);
		else
		{
			found = this->zNearWorldActors.find(AR->zActor);
			if( found != this->zNearWorldActors.end() )
				this->zNearWorldActors.erase(found);
		}
	}
}