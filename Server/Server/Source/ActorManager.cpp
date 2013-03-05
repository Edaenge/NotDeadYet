#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include "BioActor.h"
#include "WorldActor.h"
#include "ProjectileActor.h"

ActorManager::ActorManager( ActorSynchronizer* syncher ) : 
	zSynch(syncher)
{
	AddObserver(this->zSynch);
}

ActorManager::~ActorManager()
{
	this->RemoveObserver(this->zSynch);

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

	if( actor->CanCollide() )
		this->zCollideableActors.insert(actor);

	ActorAdded e;
	e.zActor = actor;
	NotifyObservers(&e);
}

void ActorManager::RemoveActor( Actor* actor )
{
	std::set<Actor*>::iterator ret = zActors.end();

	if(!actor)
		return;

	this->zActors.erase(actor);
	
	if( actor->CanCollide() )
	{
		this->zCollideableActors.erase(actor);
	}

	ActorRemoved e;
	e.zActor = actor;
	NotifyObservers(&e);

	delete actor;
}

Actor* ActorManager::CheckCollisions( Actor* actor, float& range )
{
	if(!actor)
		return NULL;

	float radiusWithin = 2.0f + range;
	Actor* collide = NULL;
	PhysicsCollisionData data;
	std::set<Actor*> inRadius;
	GetActorsInCircle(actor->GetPosition().GetXZ(), radiusWithin, inRadius);
	
	for (auto it = inRadius.begin(); it != inRadius.end(); it++)
	{
		//If same, ignore
		if((*it) == actor)
			continue;
		
		BioActor* tBioActor = dynamic_cast<BioActor*>(*it);
		Actor* target = NULL;

		//if BioActor
		if(tBioActor)
		{
			if(tBioActor->IsAlive())
				target = tBioActor;
		}
		//If Actor has Physics
		else if( (*it) && (*it)->GetPhysicsObject() )
		{
			target = (*it);
		}

		//If the target is null, ignore
		if(!target)
			continue;

		Vector3 offset = Vector3(0.0f, 0.0f, 0.0f);

		if (BioActor* bActor = dynamic_cast<BioActor*>(actor))
		{
			offset = bActor->GetCameraOffset();
		}
		else if ( ProjectileActor* projActor = dynamic_cast<ProjectileActor*>(actor) )
		{
			if( target ==  projActor->GetOwner() )
				continue;

// 			PhysicsObject* actorPhys = actor->GetPhysicsObject();
// 			Vector3 center = actorPhys->GetBoundingSphere().center;
// 			center = actorPhys->GetWorldMatrix() * center;
// 			offset = ( center - actor->GetPosition() ) * 2;

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

		BioActor* tBioActor = dynamic_cast<BioActor*>(*it);
		Actor* target = NULL;

		//if BioActor
		if(tBioActor)
		{
			if(tBioActor->IsAlive())
				target = tBioActor;
		}
		//If Actor has Physics
		else if( (*it) && (*it)->GetPhysicsObject() )
		{
			target = (*it);
		}

		//If the target is null, ignore
		if(!target)
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

unsigned int ActorManager::GetActorsInCircle( const Vector2& center, float radius, std::set<Actor*>& out) const
{
	unsigned int counter=0;

	for(auto i = zActors.cbegin(); i != zActors.cend(); i++)
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

	for(auto i = zActors.cbegin(); i != zActors.cend(); i++)
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

	for(auto i = zCollideableActors.cbegin(); i != zCollideableActors.cend(); i++)
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

void ActorManager::OnEvent( Event* e )
{
	if( ActorAdded* AA = dynamic_cast<ActorAdded*>(e) )
	{
		AddActor(AA->zActor);
	}

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
