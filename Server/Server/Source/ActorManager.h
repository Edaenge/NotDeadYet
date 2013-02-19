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
	virtual ~ActorManager();
	void AddActor( Actor* actor );
	void RemoveActor( Actor* actor );
	std::set<Actor*>& GetActors() {return this->zActors;}
	Actor* GetActor(const unsigned int Index) const;
	/*! Checks if this actor collides with BioActors in the game using physics calculations.*/
	Actor* CheckCollisions(Actor* actor, float& range);
	/*! Checks if this actor collides with BioActors in the game using distance calculations.*/
	Actor* CheckCollisionsByDistance(Actor* actor, float& range);
};