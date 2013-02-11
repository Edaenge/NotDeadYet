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

void ActorManager::AddActor( Actor* actor )
{
	actor->AddObserver(this->zSynch);
	zActors.insert(actor);

	if(WorldActor* WA = dynamic_cast<WorldActor*>(actor))
		return;

	ActorAdded e;
	e.zActor = actor;
	NotifyObservers(&e);
	
}

void ActorManager::RemoveActor( Actor* actor )
{
	this->zActors.erase(actor);

	if(WorldActor* WA = dynamic_cast<WorldActor*>(actor))
		return;

	ActorRemoved e;
	e.zActor = actor;
	NotifyObservers(&e);
}

Actor* ActorManager::CheckCollisions(Actor* player, float& range)
{
	Actor* collision;
	PhysicsCollisionData data;

	for (auto it = this->zActors.begin(); it != this->zActors.end(); it++)
	{
		if ((*it) != player)
		{
			PhysicsObject* pOtherObject = (*it)->GetPhysicsObject();
			data = GetPhysics()->GetCollisionRayMesh(player->GetPosition(), player->GetDir(), pOtherObject);

			if (data.collision && data.distance < range)
			{
				if (BioActor* player = dynamic_cast<BioActor*>((*it)))
				{
					range = data.distance;
					collision = (*it);
				}
			}
		}
	}
	return collision;
}