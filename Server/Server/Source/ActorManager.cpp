#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
ActorManager::ActorManager( ActorSynchronizer* syncher ) : 
	zSynch(syncher)
{
	AddObserver(this->zSynch);
}

void ActorManager::AddActor( Actor* actor )
{
	ActorAdded e;

	actor->AddObserver(this->zSynch);
	zActors.insert(actor);

	e.zActor = actor;
	NotifyObservers(&e);;
}

void ActorManager::RemoveActor( Actor* actor )
{
	ActorRemoved e;
	this->zActors.erase(actor);

	e.zActor = actor;
	NotifyObservers(&e);
}
