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
		for( unsigned int x=0; x<5; ++x )
		{
			// Sector Corners
			Vector3 pos;
			pos.x = (float)coords.x * FSECTOR_WORLD_SIZE;
			pos.y = 0.0f;
			pos.z = (float)coords.y * FSECTOR_WORLD_SIZE;
			
			// Randomize Position
			pos.x += (float)rand() / (float)RAND_MAX * FSECTOR_WORLD_SIZE;
			pos.z += (float)rand() / (float)RAND_MAX * FSECTOR_WORLD_SIZE;

			// Get Intersecting AI Nodes
			std::set<Vector2> intersectingNodes;
			zWorld->GetAINodesInCircle(pos.GetXZ(), 0.8f, intersectingNodes);

			// Check blocked
			bool blocked = false;
			for (auto i = intersectingNodes.cbegin(); i != intersectingNodes.cend() && !blocked; ++i )
			{
				if ( zWorld->IsBlockingAt(*i) )
				{
					blocked = true;
				}
			}

			// Don't grow at blocked positions
			if ( blocked )
			{
				continue;
			}

			// Grow only on certain textures
			float growthFactor = 0.0f;
			growthFactor += zWorld->GetAmountOfTexture(pos.GetXZ(), "01_v02-Moss.png");
			growthFactor += zWorld->GetAmountOfTexture(pos.GetXZ(), "06_v01-MossDark.png");
			growthFactor += zWorld->GetAmountOfTexture(pos.GetXZ(), "07_v01-MossLight.png");
			growthFactor += zWorld->GetAmountOfTexture(pos.GetXZ(), "02_v03-Leaf.png");

			// Only grow on grass
			if ( growthFactor < 0.5f )
			{
				continue;
			}

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

			// Bush has been placed, abort
			break;
		}
	}
}