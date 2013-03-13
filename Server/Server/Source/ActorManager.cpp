#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include "BioActor.h"
#include "ItemActor.h"
#include "SupplyActor.h"
#include "WorldActor.h"
#include "ProjectileActor.h"

ActorManager::ActorManager( ActorSynchronizer* syncher, SoundHandler* sh) : 
	zSynch(syncher), zSoundHandler(sh)
{
	AddObserver(this->zSynch);
	AddObserver(this->zSoundHandler);
}

ActorManager::~ActorManager()
{
	this->RemoveObserver(this->zSynch);
	this->RemoveObserver(this->zSoundHandler);

	auto it_actors_end = this->zActors.end();
	for(auto it = this->zActors.begin(); it != it_actors_end; it++)
	{
		Actor* temp = (*it);
		SAFE_DELETE(temp);
	}
	this->zActors.clear();

	// Delete Behaviors
	for( auto i = this->zBehaviors.begin(); i != this->zBehaviors.end(); ++i )
	{
		Behavior* data = (*i);
		SAFE_DELETE(data);
	}
	this->zBehaviors.clear();
}

void ActorManager::AddActor( Actor* actor )
{
	if(!actor)
		return;
	actor->AddObserver(this->zSoundHandler);
	zActors.insert(actor);

	if( actor->CanCollide() )
		this->zCollideableActors.insert(actor);

	if (dynamic_cast<ItemActor*>(actor) || dynamic_cast<BioActor*>(actor) || dynamic_cast<SupplyActor*>(actor))
		zLootableActors.insert(actor);

	ActorAdded e;
	e.zActor = actor;
	NotifyObservers(&e);
}

void ActorManager::AddBehavior( Behavior* behavior )
{
	if(!behavior)
		return;

	this->zBehaviors.insert(behavior);
	this->AddObserver(behavior);
}

void ActorManager::RemoveActor( Actor* actor )
{
	if(!actor)
		return;

	this->zActors.erase(actor);
	this->zCollideableActors.erase(actor);
	this->zLootableActors.erase(actor);

	ActorRemoved e;
	e.zActor = actor;
	NotifyObservers(&e);

	delete actor;
} 

void ActorManager::RemoveBehavior(Actor* actor, bool instantRemove)
{
	auto it_zBehavior_end = this->zBehaviors.end();

	for (auto it = this->zBehaviors.begin(); it != it_zBehavior_end;)
	{
		if ( (*it)->GetActor() == actor )
		{
			Behavior* temp = *it;
			if (instantRemove)
			{
				it = this->zBehaviors.erase(it);
				RemoveObserver(temp);
				SAFE_DELETE(temp);

				it = it_zBehavior_end;
			}
			else
			{
				temp->Remove();
			}
		}
		else
		{
			it++;
		}
	}

}

void ActorManager::RemoveBehavior( Behavior* behavior )
{
	if(!behavior)
		return;

	this->RemoveObserver(behavior);

	this->zBehaviors.erase(behavior);
	SAFE_DELETE(behavior);
}

Actor* ActorManager::CheckCollisions( Actor* actor, float& range, const std::set<Actor*>& actors )
{
	if(!actor)
		return NULL;

	float rangeWithin = 1.0f + range;
	Actor* collide = NULL;
	PhysicsCollisionData data;
	Vector3 pos = actor->GetPosition();
	Vector3 offset = Vector3(0.0f, 0.0f, 0.0f);

	//Need offset if bioActor
	if ( BioActor* bActor = dynamic_cast<BioActor*>(actor) )
		offset = bActor->GetCameraOffset();
	
	auto it_end = actors.end();
	for (auto it = actors.begin(); it != it_end; it++)
	{
		if( (*it) == actor )
			continue;
		if( !(*it)->CanCollide() )
			continue;

		float distance = ( pos.GetXZ() - (*it)->GetPosition().GetXZ() ).GetLength();

		if(distance > rangeWithin)
			continue;

		PhysicsObject* targetObject = (*it)->GetPhysicsObject();
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

Actor* ActorManager::CheckCollisions( Actor* actor, float& range )
{
	return CheckCollisions(actor, range, this->zCollideableActors);
}

Actor* ActorManager::GetActor( const unsigned int ID ) const
{
	auto it_actors_end = this->zActors.end();
	for(auto it_Actor = this->zActors.begin(); it_Actor != it_actors_end; it_Actor++)
	{
		if((*it_Actor)->GetID() == ID)
		{
			return (*it_Actor);
		}
	}
	return NULL;
}

unsigned int ActorManager::GetActorsInCircle( const Vector2& center, float radius, std::set<Actor*>& out) const
{
	unsigned int counter=0;

	auto it_zActors_end = zActors.cend();
	for(auto i = zActors.cbegin(); i != it_zActors_end; i++)
	{
		Vector2 pos( (*i)->GetPosition().x, (*i)->GetPosition().z );
		if( Vector2(center-pos).GetLength() < radius)
		{
			out.insert(*i);
			counter++;
		}
	}

	return counter;
}

unsigned int ActorManager::GetActorsInCircle( const Vector2& center, float radius, std::set<Actor*>& out, const unsigned int filter ) const
{
	unsigned int counter=0;

	auto it_zActors_end = zActors.cend();
	for(auto i = zActors.cbegin(); i != it_zActors_end; i++)
	{
		if( (*i)->GetType() == filter )
		{
			Vector2 pos( (*i)->GetPosition().x, (*i)->GetPosition().z );
			if( Vector2(center-pos).GetLength() < radius)
			{
				out.insert(*i);
				counter++;
			}
		}
	}

	return counter;
}

unsigned int ActorManager::GetCollideableActorsInCircle( const Vector2& center, float radius, std::set<Actor*>& out )
{
	unsigned int counter=0;

	auto i_zCollideableActors_end = zCollideableActors.cend();
	for(auto i = zCollideableActors.cbegin(); i != i_zCollideableActors_end; i++)
	{
		Vector2 pos( (*i)->GetPosition().x, (*i)->GetPosition().z );
		if( Vector2(center-pos).GetLength() < radius)
		{
			out.insert(*i);
			counter++;
		}
		
	}

	return counter;
}

std::set<Actor*>& ActorManager::GetLootableActors()
{
	return this->zLootableActors;
}

void ActorManager::ClearAll()
{
	PhysicsEngine* physics = GetPhysics();
	auto it = zActors.begin();
	while ( it != zActors.end())
	{
		PhysicsObject* pObj = (*it)->GetPhysicsObject();

		if(pObj)
			physics->DeletePhysicsObject(pObj);
		
		Actor* actor = (*it);
		SAFE_DELETE(actor);

		it++;
	}
	zActors.clear();
	zCollideableActors.clear();

}