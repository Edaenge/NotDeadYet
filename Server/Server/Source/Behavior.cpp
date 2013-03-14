#include <world/World.h>
#include "Behavior.h"
#include "Actor.h"
#include "WorldActor.h"
#include "BioActor.h"
#include "ActorSynchronizer.h"
#include "ActorManager.h"

using namespace std::chrono;


Behavior::Behavior( Actor* actor, World* world ) :
	zActor(actor),
	zWorld(world),
	zAnchor(0),
	zAwakeTime(time_point_cast<microseconds>(system_clock::now()))
{
	zActor->AddObserver(this);

	if ( !zAnchor )
	{
		zAnchor = zWorld->CreateAnchor();
		zAnchor->position = zActor->GetPosition().GetXZ();
		zAnchor->radius = 32.0f;
	}

	this->zRemoveBehavior = false;
	this->zNearActorsIndex = 0;
	this->zNearByRadius = 50.0f;
}

Behavior::~Behavior()
{
	if ( zAnchor && zWorld ) 
		zWorld->DeleteAnchor(zAnchor);

	if (zActor)
		zActor->RemoveObserver(this);

	this->zNearActors.clear();
	this->zNearDynamicActors.clear();
	this->zNearStaticActors.clear();

}

void Behavior::Sleep(float time)
{
	zAwakeTime = time_point_cast<std::chrono::microseconds>(system_clock::now()) + std::chrono::microseconds((long long)(time * 1000000.0f));
}

bool Behavior::IsAwake() const
{
	return time_point_cast<microseconds>(system_clock::now()) >= zAwakeTime;
}

void Behavior::OnEvent(Event* e)
{
	if ( ActorPositionEvent* APE = dynamic_cast<ActorPositionEvent*>(e) )
	{
		zAnchor->position = APE->zActor->GetPosition().GetXZ();
	}
	else if ( ActorDeleteEvent* ADE = dynamic_cast<ActorDeleteEvent*>(e) )
	{
		zWorld->DeleteAnchor(zAnchor);
		zActor = NULL;
	}
	else if( ActorRemovedEvent* AR = dynamic_cast<ActorRemovedEvent*>(e) )
	{
		auto found = this->zNearActors.find(AR->zActor);
		if(found != this->zNearActors.end())
			this->zNearActors.erase(found);

		found = this->zNearDynamicActors.find(AR->zActor);
		if( found != this->zNearDynamicActors.end() )
			this->zNearDynamicActors.erase(found);
		else
		{
			found = this->zNearStaticActors.find(AR->zActor);
			if( found != this->zNearStaticActors.end() )
				this->zNearStaticActors.erase(found);
		}
	}
}

bool Behavior::Update( float )
{
	return ( zActor == NULL );
}

bool Behavior::RefreshNearCollideableActors( const std::set<Actor*>& actors )
{
	if( !this->zActor || !this->zActor->CanCollide() )
		return NULL;

	unsigned int size = actors.size();
	// Increment 10%
	unsigned int increment = (unsigned int)(size * 0.10);

	if(increment == 0)
		increment = size;

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
			WorldActor* worldActor = dynamic_cast<WorldActor*>(*it);

			auto found = this->zNearActors.find(*it);

			if( vec.GetLength() <= zNearByRadius )
			{
				if( found == zNearActors.end() )
					zNearActors.insert(*it);

				if(worldActor && zNearStaticActors.find(*it) == zNearStaticActors.end())
					this->zNearStaticActors.insert(*it);

				else if(!worldActor && zNearDynamicActors.find(*it) == zNearDynamicActors.end())
					this->zNearDynamicActors.insert(*it);
			}
			else
			{
				if( found != zNearActors.end() )
					zNearActors.erase(*it);

				if( worldActor && zNearStaticActors.find(*it) != zNearStaticActors.end() )
					this->zNearStaticActors.erase(*it);

				else if(!worldActor && zNearDynamicActors.find(*it) != zNearDynamicActors.end() )
					this->zNearDynamicActors.erase(*it);
			}
		}

		zNearActorsIndex++;
		it++;
	}

	return true;
}

Actor* Behavior::DistanceCollision(const Actor* actor, const float& radius, const std::set<Actor*>& actors )
{
	if( !actor || !actor->CanCollide() )
		return NULL;

	Vector3 pos = actor->GetPosition();

	auto it_end = actors.end();
	for(auto it = actors.begin(); it != it_end; it++)
	{
		float distance = ( pos - (*it)->GetPosition() ).GetLength();
		if( distance < radius + (*it)->GetCollisionRadius() )
			return (*it);
	}

	return NULL;
}

Actor* Behavior::RayVsMeshCollision( const Actor* actor, const Vector3& rayOrigin, float& range, const std::set<Actor*>& actors )
{
	if( !actor || !actor->CanCollide() )
		return NULL;

	float rangeWithin = 1.0f + range;
	Actor* collide = NULL;
	PhysicsCollisionData data;
	Vector3 pos = actor->GetPosition();
	Vector3 dir = actor->GetDir();

	auto it_end = actors.end();
	for (auto it = actors.begin(); it != it_end; it++)
	{
		if( *it == actor )
			continue;
		if( !(*it)->CanCollide() )
			continue;

		float distance = ( pos.GetXZ() - (*it)->GetPosition().GetXZ() ).GetLength();

		if(distance > rangeWithin)
			continue;

		PhysicsObject* targetObject = (*it)->GetPhysicsObject();
		data = GetPhysics()->GetCollisionRayMesh(rayOrigin, dir, targetObject);

		if(data.collision && data.distance < range)
		{
			range = data.distance;
			collide = (*it);
		}

	}

	//Returns the closest actor
	return collide;
}

Actor* Behavior::DistanceStaticActorCollision()
{
	if( !this->zActor || !this->zActor->CanCollide() )
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
		if( this->zWorld->IsBlockingAt(blockedPos) )
			isBlocking = true;
	}

	if( !isBlocking )
		return NULL;

	return DistanceCollision(this->zActor, this->zActor->GetCollisionRadius(), zNearStaticActors);
}

Actor* Behavior::DistanceDynamicActorCollision()
{
	if( !this->zActor || !this->zActor->CanCollide() )
		return NULL;

	return DistanceCollision(this->zActor, this->zActor->GetCollisionRadius(), zNearDynamicActors);
}

Actor* Behavior::RayVsMeshDynamicActorCollision( float& range, const Vector3& rayOrigin )
{
	if( !this->zActor || !this->zActor->CanCollide() )
		return NULL;

	return RayVsMeshCollision(this->zActor, rayOrigin, range, zNearDynamicActors);
}

Actor* Behavior::RayVsMeshStaticActorCollision( float& range, const Vector3& rayOrigin )
{
	if( !this->zActor || !this->zActor->CanCollide() )
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
		catch(...)
		{
			return NULL;
		}
	}

	if( !isBlocking )
		return NULL;

	return RayVsMeshCollision(this->zActor, rayOrigin, range, zNearStaticActors);
}

void Behavior::SetNearStaticActors( std::set<Actor*> actors )
{
	this->zNearStaticActors = actors;
	this->zNearStaticActors.erase(this->zActor);

	this->zNearActors.clear();
	this->zNearActors = zNearDynamicActors;

	auto it_end = this->zNearStaticActors.end();
	for (auto it = this->zNearStaticActors.begin(); it != it_end; it++)
	{
		this->zNearActors.insert(*it);
	}
}

void Behavior::SetNearDynamicActors( std::set<Actor*> actors )
{
	this->zNearDynamicActors = actors;
	this->zNearDynamicActors.erase(this->zActor);

	this->zNearActors.clear();
	this->zNearActors = zNearStaticActors;

	auto it_end = this->zNearDynamicActors.end();
	for (auto it = this->zNearDynamicActors.begin(); it != it_end; it++)
	{
		this->zNearActors.insert(*it);
	}
}
