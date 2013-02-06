#include "ActorManager.h"
#include "Actor.h"
#include "ActorSynchronizer.h"
ActorManager::ActorManager( ActorSynchronizer* syncher ) : 
	zSynch(syncher)
{

}

void ActorManager::AddActor( Actor* actor )
{
	actor->AddObserver(this->zSynch);
	zActors.insert(actor);
}

void ActorManager::RemoveActor( Actor* actor )
{
	this->zActors.erase(actor);
}
