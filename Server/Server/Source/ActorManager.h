#pragma once

#include <Observer.h>

class ActorSynchronizer;
class Actor;
class Vector2;
class ActorAdded;

class ActorManager : public Observed, public Observer
{
	ActorSynchronizer *zSynch;
	std::set<Actor*> zActors;

public:
	ActorManager( ActorSynchronizer* syncher );
	virtual ~ActorManager();
	void AddActor( Actor* actor );
	/*! Removes The actor, physics object will NOT be freed.*/
	void RemoveActor( Actor* actor );
	/*! Removes all actors, physics objects WILL be freed.*/
	void ClearAll();
	std::set<Actor*>& GetActors() {return this->zActors;}
	Actor* GetActor(const unsigned int ID) const;
	/*! Adds all Actors within the circle to out. Returns total amount of actors found*/
	unsigned int GetActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out) const;
	unsigned int GetActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out, const unsigned int filter) const;

	/*! Checks if this actor collides with BioActors/WorldActors in the game using physics calculations.*/
	Actor* CheckCollisions(Actor* actor, float& range);
	/*! Checks if this actor collides with BioActors/WorldActors in the game using distance calculations.*/
	Actor* CheckCollisionsByDistance(Actor* actor, float& range);
	/**/
	virtual void OnEvent( Event* e);
};