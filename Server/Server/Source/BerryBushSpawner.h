#pragma once

#include <Vector.h>
#include <World/Observer.h>
#include <set>

class World;
class ActorManager;


class BerryBushSpawner : public Observer
{
	World* zWorld;
	ActorManager* zActorManager;
	std::set< Vector2UINT > zProcessedSectors;

public:
	BerryBushSpawner(World* world, ActorManager* actorManager);
	virtual ~BerryBushSpawner();

private:
	void OnEvent( Event* event );
	void ProcessSector( const Vector2UINT& coords );
};