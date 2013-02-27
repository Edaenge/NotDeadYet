//Simon Edänge, 2013-02-27
#pragma once

#include "Actor.h"
#include "Item.h"

class ActorManager;

class SupplyDrop : public Observed
{
public:
	SupplyDrop( ActorManager* actorManager );
	virtual ~SupplyDrop();
	
	bool SpawnDrop();
	bool SpawnDrop(const Vector3& pos, std::set<Item*>& items);
	
private:
};