#pragma once

#include <Observer.h>

class World;
class Actor;
class WorldAnchor;

class Behavior : public Observer
{
protected:
	Actor* zActor;
	World* zWorld;
	WorldAnchor* zAnchor;

public:
	Behavior(Actor* actor, World* world);
	virtual ~Behavior();

	inline Actor* GetActor() const { return zActor; }
	World* GetWorld() const { return zWorld; }

	// Returns true to indicate when it wants to get deleted
	virtual bool Update(float dt);

	// Handles The Anchor Update From ActorMovedEvent
	virtual void OnEvent(Event* e);
};