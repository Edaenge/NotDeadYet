#include "BerryBushSpawner.h"
#include "BerryBushActor.h"
#include "ActorManager.h"
#include <World/World.h>


BerryBushSpawner::BerryBushSpawner(World* world, ActorManager* manager) :
	zWorld(world),
	zActorManager(manager)
{
	zWorld->AddObserver(this);
}

BerryBushSpawner::~BerryBushSpawner()
{
	if ( zWorld ) zWorld->RemoveObserver(this);
}

void BerryBushSpawner::OnEvent( Event* e )
{
	if ( SectorLoadedEvent* SLE = dynamic_cast<SectorLoadedEvent*>(e) )
	{
		ProcessSector(Vector2UINT(SLE->x, SLE->y));
	}
}

void BerryBushSpawner::ProcessSector( const Vector2UINT& coords )
{
	auto i = zProcessedSectors.find(coords);

	if ( i == zProcessedSectors.cend() )
	{
		// Sector Corners
		Vector3 pos;
		pos.x = (float)coords.x * FSECTOR_WORLD_SIZE;
		pos.y = 0.0f;
		pos.z = (float)coords.y * FSECTOR_WORLD_SIZE;
		
		// Randomize Position
		pos.x += (float)rand() / (float)RAND_MAX * FSECTOR_WORLD_SIZE;
		pos.z += (float)rand() / (float)RAND_MAX * FSECTOR_WORLD_SIZE;

		// Try Terrain Height
		try
		{
			pos.y = zWorld->CalcHeightAtWorldPos(pos.GetXZ());
		}
		catch(...)
		{

		}

		// Create Berry Bush Actor
		BerryBushActor* berryBushActor = new BerryBushActor();
		berryBushActor->SetPosition(pos);

		// Spawn Bush
		zActorManager->AddActor(berryBushActor);

		// Add To Processed
		zProcessedSectors.insert(coords);
	}
}