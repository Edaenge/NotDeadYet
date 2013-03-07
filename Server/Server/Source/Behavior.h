#pragma once

#include <Observer.h>
#include <chrono>

class World;
class Actor;
class WorldAnchor;

class Behavior : public Observer
{
	typedef std::chrono::duration<float, std::ratio<1, 1>> fSeconds;

protected:
	Actor* zActor;
	World* zWorld;
	WorldAnchor* zAnchor;
	std::chrono::time_point<std::chrono::system_clock, fSeconds> zAwakeTime;

public:
	Behavior(Actor* actor, World* world);
	virtual ~Behavior();

	inline Actor* GetActor() const { return zActor; }
	inline World* GetWorld() const { return zWorld; }

	// Don't Update For Time
	void Sleep(float dt);

	// Check if asleep
	bool IsAwake() const;

	// Returns true to indicate when it wants to get deleted
	virtual bool Update(float dt);

	// Handles The Anchor Update From ActorMovedEvent
	virtual void OnEvent(Event* e);
};