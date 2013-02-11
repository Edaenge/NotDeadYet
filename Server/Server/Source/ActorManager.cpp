#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include "BioActor.h"

ActorManager::ActorManager( ActorSynchronizer* syncher ) : 
	zSynch(syncher)
{
	AddObserver(this->zSynch);
}

void ActorManager::AddActor( Actor* actor )
{
	actor->AddObserver(this->zSynch);
	zActors.insert(actor);

	ActorAdded e;
	e.zActor = actor;
	NotifyObservers(&e);
}

void ActorManager::RemoveActor( Actor* actor )
{
	this->zActors.erase(actor);

		ActorRemoved e;
	e.zActor = actor;
	NotifyObservers(&e);
}

Actor* ActorManager::CheckCollisions(Actor* player)
{
	Actor* collision;
	PhysicsCollisionData data;
	float leastDistance = 2.0f;
	for (auto it = this->zActors.begin(); it != this->zActors.end(); it++)
	{
		if ((*it) != player)
		{
			PhysicsObject* pOtherObject = (*it)->GetPhysicsObject();
			data = GetPhysics()->GetCollisionRayMesh(player->GetPosition(), player->GetDir(), pOtherObject);

			if (data.collision && data.distance < leastDistance)
			{
				if (BioActor* player = dynamic_cast<BioActor*>((*it)))
				{
					leastDistance = data.distance;
					collision = (*it);
				}
			}
		}
	}
	return collision;
}