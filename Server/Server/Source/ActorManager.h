#pragma once

#include <Observer.h>
#include "SoundHandler.h"
#include "Behavior.h"

class ActorSynchronizer;
class Actor;
class Vector2;

class ActorManager : public Observed, public Observer
{
	ActorSynchronizer *zSynch;
	SoundHandler* zSoundHandler;
	std::set<Actor*> zActors;
	std::set<Actor*> zCollideableActors;
	std::set<Actor*> zLootableActors;
	std::set<Behavior*> zBehaviors;

public:
	ActorManager( ActorSynchronizer* syncher, SoundHandler* sh);
	virtual ~ActorManager();
	void AddActor( Actor* actor );
	void AddBehavior( Behavior* behavior );
	/*! Removes The actor, physics object will NOT be freed.*/
	void RemoveActor( Actor* actor );
	void RemoveBehavior( Behavior* behavior );
	void RemoveBehavior( Actor* actor);
	/*! Removes all actors, behaviors, physics objects WILL be freed.*/
	void ClearAll();

	std::set<Actor*>& GetActors() {return this->zActors;}
	std::set<Behavior*>& GetBehaviors() {return this->zBehaviors;}
	std::set<Actor*>& GetCollideableActors() {return this->zCollideableActors;}

	Actor* GetActor(const unsigned int ID) const;
	/*! Adds all Actors within the circle to out. Returns total amount of actors found*/
	unsigned int GetActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out) const;
	unsigned int GetActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out, const unsigned int filter) const;
	/*! */
	unsigned int GetCollideableActorsInCircle(const Vector2& center, float radius, std::set<Actor*>& out);

	std::set<Actor*>& GetLootableActors();

	/*! Checks if this actor collides with BioActors/WorldActors in the game using physics calculations.*/
	Actor* CheckCollisions(Actor* actor, float& range);
	/*! Checks if this actor collides with BioActors/WorldActors in the game using distance calculations.*/
	Actor* CheckCollisionsByDistance(Actor* actor, float& range);
};