#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include "BioActor.h"
#include "ItemActor.h"
#include "SupplyActor.h"
#include "BerryBushActor.h"
#include "WorldActor.h"
#include "ProjectileActor.h"
#include "Behavior.h"


ActorManager::ActorManager(ActorSynchronizer* syncher) : 
	zSynch(syncher)
{
	AddObserver(this->zSynch);
}

ActorManager::~ActorManager()
{
	// Remove Observer
	RemoveObserver(this->zSynch);

	// Delete Behaviors
	for( auto i = this->zBehaviors.begin(); i != this->zBehaviors.end(); ++i )
	{
		Behavior* data = (*i);
		RemoveObserver(data);
		SAFE_DELETE(data);
	}

	// Clear Behavior Set
	this->zBehaviors.clear();

	// Delete Actors
	auto it_actors_end = this->zActors.end();
	for(auto it = this->zActors.begin(); it != it_actors_end; it++)
	{
		Actor* temp = (*it);
		SAFE_DELETE(temp);
	}

	// Clear Actor Set
	this->zActors.clear();
}

void ActorManager::AddActor( Actor* actor )
{
	if(!actor)
		return;

	zActors.insert(actor);

	if( actor->CanCollide() )
		this->zCollideableActors.insert(actor);

	if (dynamic_cast<ItemActor*>(actor) || dynamic_cast<BioActor*>(actor) || dynamic_cast<SupplyActor*>(actor) || dynamic_cast<BerryBushActor*>(actor))
		zLootableActors.insert(actor);

	// Notify Observers
	ActorAddedEvent e;
	e.zActor = actor;
	NotifyObservers(&e);
}

void ActorManager::AddBehavior( Behavior* behavior )
{
	if(!behavior)
		return;

	// Notify Observers
	BehaviorAddedEvent BAE;
	BAE.zBehavior = behavior;
	NotifyObservers(&BAE);

	this->zBehaviors.insert(behavior);

	// TODO: Move Me, Actor manager should not observe every actor
	this->AddObserver(behavior);
}

void ActorManager::RemoveActor( Actor* actor )
{
	if(!actor)
		return;

	if( SupplyActor* sActor =  dynamic_cast<SupplyActor*>(actor) )
	{
		if( sActor->HasParachute() )
		{
			Actor* parachute = sActor->GetParachute();
			RemoveActor(parachute);
		}
	}

	this->zActors.erase(actor);
	this->zCollideableActors.erase(actor);
	this->zLootableActors.erase(actor);

	// Notify Observers
	ActorRemovedEvent e;
	e.zActor = actor;
	NotifyObservers(&e);

	if( actor->CanCollide() )
	{
		PhysicsObject* pObj = actor->GetPhysicsObject();
		GetPhysics()->DeletePhysicsObject( pObj );
		actor->SetPhysicsObject(NULL);
	}

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

				// Notify Observers
				BehaviorRemovedEvent BRE;
				BRE.zBehavior = temp;
				NotifyObservers(&BRE);

				// Remove Me As Observer
				RemoveObserver(temp);

				// Delete Behavior
				SAFE_DELETE(temp);

				// Next Iteration
				it = it_zBehavior_end;
			}
			else
			{
				temp->Remove();
				it++;
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

	// TODO: Move me, actor manager should not observer every behavior
	this->RemoveObserver(behavior);

	// Remove from set
	this->zBehaviors.erase(behavior);

	// Delete
	SAFE_DELETE(behavior);
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

unsigned int ActorManager::GetActorsInCircle( const Vector2& center, float radius, std::set<Actor*>& out, const unsigned int filter ) const
{
	unsigned int counter=0;

	auto it_zActors_end = zActors.cend();
	for(auto i = zActors.cbegin(); i != it_zActors_end; i++)
	{
		if( !filter || (*i)->GetType() == filter )
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