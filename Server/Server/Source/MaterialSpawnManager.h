/*
Material Drop Manager, Handles the material drops from world
*/

#pragma once

#include <World/Observer.h>
#include <Vector.h>

class World;
class ActorManager;
class Entity;


class MaterialSpawnManager : public Observer
{
	World* zWorld;
	ActorManager* zActorManager;
	std::set<Vector2UINT> zProcessedSectors;

public:
	MaterialSpawnManager(World* world, ActorManager* manager);
	virtual ~MaterialSpawnManager();

protected:
	virtual void OnEvent(Event* e);
	
	// Process a sector for spawns
	void ProcessSector(const Vector2UINT& sectorCoords);

	// Process an entity for spawns
	void ProcessEntity(const Entity* entity);
};
