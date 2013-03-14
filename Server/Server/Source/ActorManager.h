#pragma once

#include <World/Observer.h>
#include <Vector.h>
#include "BehaviorManager.h"

class ActorSynchronizer;
class Actor;
class Behavior;


class ActorAddedEvent : public Event
{
public:
	Actor* zActor;
};

class ActorRemovedEvent : public Event
{
public:
	Actor* zActor;
};

class ActorManager : public Observed, public Observer
{
	ActorSynchronizer *zSynch;
	std::set<Actor*> zActors;
	std::set<Actor*> zCollideableActors;
	std::set<Actor*> zLootableActors;
	std::set<Behavior*> zBehaviors;

public:
	ActorManager( ActorSynchronizer* syncher );
	virtual ~ActorManager();

	void AddActor( Actor* actor );
	void AddBehavior( Behavior* behavior );

	/*! Removes The actor, physics object will NOT be freed.*/
	void RemoveActor( Actor* actor );
	void RemoveBehavior(Behavior* behavior );
	void RemoveBehavior(Actor* actor, bool instant = true);

	/*! Removes all actors, behaviors, physics objects WILL be freed.*/
	void ClearAll();

	inline std::set<Actor*>& GetActors() { return this->zActors; }
	inline std::set<Behavior*>& GetBehaviors() { return this->zBehaviors; }
	inline std::set<Actor*>& GetCollideableActors() { return this->zCollideableActors; }

	// Returns actor matching ID
	Actor* GetActor(const unsigned int ID) const;

	/*! Adds all Actors within the circle to out. Returns total amount of actors found*/
	unsigned int GetActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out, const unsigned int filter=0) const;
	
	// Finds collideable Actors In Circle
	unsigned int GetCollideableActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out);

	// Returns set of lootable actors
	std::set<Actor*>& GetLootableActors();

	/*! Checks if this actor collides with BioActors/WorldActors in the game using physics calculations.*/
	Actor* CheckCollisions(Actor* actor, float& range);
	Actor* CheckCollisions(Actor* actor, float& range, const std::set<Actor*>& actors);
};