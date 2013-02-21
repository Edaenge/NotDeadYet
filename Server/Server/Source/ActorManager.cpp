#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include "BioActor.h"
#include "WorldActor.h"

ActorManager::ActorManager( ActorSynchronizer* syncher ) : 
	zSynch(syncher)
{
	AddObserver(this->zSynch);
}

ActorManager::~ActorManager()
{
	for(auto it = this->zActors.begin(); it != this->zActors.end(); it++)
	{
		Actor* temp = (*it);
		SAFE_DELETE(temp);
	}
	this->zActors.clear();
}

void ActorManager::AddActor( Actor* actor )
{
	if(!actor)
		return;

	zActors.insert(actor);

	ActorAdded e;
	e.zActor = actor;
	NotifyObservers(&e);
}

void ActorManager::RemoveActor( Actor* actor )
{
	if(!actor)
		return;

	this->zActors.erase(actor);

	ActorRemoved e;
	e.zActor = actor;
	NotifyObservers(&e);

	delete actor;
}

Actor* ActorManager::CheckCollisions( Actor* actor, float& range )
{
	if(!actor)
		return NULL;

	Actor* collide = NULL;
	PhysicsCollisionData data;
	float rangeWithin = 2.0f + range;
	Vector3 offset = Vector3(0.0f, 0.0f, 0.0f);

	for (auto it = this->zActors.begin(); it != this->zActors.end(); it++)
	{
		//If same, ignore
		if((*it) == actor)
			continue;
		
		//If not BioActor, ignore
		BioActor* target = dynamic_cast<BioActor*>(*it);
		if(!target)
			continue;
		
		//If the BioActor is dead, ignore
		if(!target->IsAlive())
			continue;

		//check length, ignore if too far.
		float length = ( actor->GetPosition() - (*it)->GetPosition() ).GetLength();
		if(length > rangeWithin)
			continue;

		if (BioActor* bActor = dynamic_cast<BioActor*>(actor))
		{
			offset = bActor->GetCameraOffset();
		}

		PhysicsObject* targetObject = target->GetPhysicsObject();
		data = GetPhysics()->GetCollisionRayMesh(actor->GetPosition() + offset, actor->GetDir(), targetObject);

		if(data.collision && data.distance < range)
		{
			range = data.distance;
			collide = (*it);
		}
		
	}

	//Returns the closest actor
	return collide;
}

Actor* ActorManager::CheckCollisionsByDistance( Actor* actor, float& range )
{
	if(!actor)
		return NULL;

	Actor* collide = NULL;
	float distance = 0.0f;

	for (auto it = this->zActors.begin(); it != this->zActors.end(); it++)
	{
		//If same, ignore
		if((*it) == actor)
			continue;

		//If not BioActor, ignore
		BioActor* target = dynamic_cast<BioActor*>(*it);
		if(!target)
			continue;

		//If the BioActor is dead, ignore
		if(!target->IsAlive())
			continue;

		//Check distance
		distance = ( actor->GetPosition() - (*it)->GetPosition() ).GetLength();
		if(distance <= range)
		{
			range = distance;
			collide = (*it);
		}
	}

	//Returns the closest actor
	return collide;
}

Actor* ActorManager::GetActor( const unsigned int ID ) const
{
	for(auto it_Actor = this->zActors.begin(); it_Actor != this->zActors.end(); it_Actor++)
	{
		if((*it_Actor)->GetID() == ID)
		{
			return (*it_Actor);
		}
	}
	return NULL;
}