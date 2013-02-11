#pragma once

#include <Observer.h>

class ActorSynchronizer;
class Actor;

class ActorManager : public Observed, public Observer
{
	ActorSynchronizer *zSynch;
	std::set<Actor*> zActors;

public:
	ActorManager( ActorSynchronizer* syncher );
	void AddActor( Actor* actor );
	void RemoveActor( Actor* actor );
	std::set<Actor*>& GetActors() {return this->zActors;}
	Actor* GetActor(const unsigned int Index) const;
	Actor* SearchAndGetActor(const unsigned int ID) const;
	Actor* CheckCollisions(Actor* player);
};