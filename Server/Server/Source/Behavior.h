#pragma once

#include <World/Observer.h>
#include <chrono>
#include "Physics.h"

class World;
class Actor;
class WorldAnchor;


class Behavior : public Observer
{
protected:
	Actor* zActor;
	World* zWorld;
	WorldAnchor* zAnchor;

	std::set<Actor*> zNearActors;
	std::set<Actor*> zNearStaticActors;
	std::set<Actor*> zNearDynamicActors;

	std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> zAwakeTime;
	bool zRemoveBehavior;
	float zNearByRadius;
	unsigned int zNearActorsIndex;

protected:
	virtual Actor* RayVsMeshDynamicActorCollision( float& range, const Vector3& rayOrigin );
	virtual Actor* RayVsMeshStaticActorCollision( float& range, const Vector3& rayOrigin );

	virtual Actor* DistanceStaticActorCollision();
	virtual Actor* DistanceDynamicActorCollision();

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

	inline bool Removed() {return this->zRemoveBehavior;}
	inline void Remove() {this->zRemoveBehavior = true;}

	inline std::set<Actor*> GetNearActors() const {return this->zNearActors;}
	inline std::set<Actor*> GetNearDynamicActors() const {return this->zNearDynamicActors;}
	inline std::set<Actor*> GetNearStaticActors() const {return this->zNearStaticActors;}
	inline float GetNearByRadius() const {return this->zNearByRadius;}

	void SetNearStaticActors(std::set<Actor*> actors);
	void SetNearDynamicActors(std::set<Actor*> actors);

	virtual bool RefreshNearCollideableActors(const std::set<Actor*>& actors);

	/*! Check collisions using distance.*/
	virtual Actor* DistanceCollision( const Actor* actor, const float& radius, const std::set<Actor*>& actors);
	/*! Check collisions using ray vs mesh.*/
	virtual Actor* RayVsMeshCollision( const Actor* actor, const Vector3& rayOrigin, float& range, const std::set<Actor*>& actors );
};