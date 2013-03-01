//Simon Edänge, 2013-02-27
#pragma once

#include "Actor.h"
#include "Item.h"

class ActorManager;
class World;
class PhysicsEngine;

class SupplyDrop : public Observed
{
public:
	SupplyDrop( ActorManager* actorManager, World* world );
	virtual ~SupplyDrop();
	
	bool SpawnSupplyDrop( Vector2& pos, std::set<Item*>& items);
	bool SpawnSupplyDrop( Vector2& pos, std::set<Item*>& items, const unsigned int itemCapacity);

	bool SpawnAirbornSupplyDrop( Vector3& pos, std::set<Item*>& items);
	bool SpawnAirbornSupplyDrop( Vector3& pos, std::set<Item*>& items, const unsigned int itemCapacity);

private:
	World* zWorld;
	ActorManager* zActorManager;
	PhysicsEngine* zPhyEngine;
};