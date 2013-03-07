#include "MaterialSpawnManager.h"
#include "ActorManager.h"
#include "MaterialSpawnSet.h"
#include "ItemActor.h"
#include <ItemLookup.h>
#include <Material.h>
#include <World/World.h>
#include <World/Entity.h>


MaterialSpawnManager::MaterialSpawnManager(World* world, ActorManager* manager) :
	zWorld(world),
	zActorManager(manager)
{
	zWorld->AddObserver(this);
}

MaterialSpawnManager::~MaterialSpawnManager()
{
	if ( zWorld ) zWorld->RemoveObserver(this);
}

void MaterialSpawnManager::OnEvent(Event* e)
{
	if ( WorldDeletedEvent* WDE = dynamic_cast<WorldDeletedEvent*>(e) )
	{
		zWorld = 0;
	}
	else if ( SectorLoadedEvent* SLE = dynamic_cast<SectorLoadedEvent*>(e) )
	{
		Vector2UINT sectorCoords(SLE->x, SLE->y);
		auto i = zProcessedSectors.find(sectorCoords);
		if ( i == zProcessedSectors.end() )
		{
			ProcessSector(sectorCoords);
			zProcessedSectors.insert(sectorCoords);
		}
	}
}

void MaterialSpawnManager::ProcessSector(const Vector2UINT& sectorCoords)
{
	// Sector Rectangle
	Rect sectorArea;
	sectorArea.topLeft.x = (float)sectorCoords.x * FSECTOR_WORLD_SIZE;
	sectorArea.topLeft.y = (float)sectorCoords.y * FSECTOR_WORLD_SIZE;
	sectorArea.size.x = FSECTOR_WORLD_SIZE;
	sectorArea.size.y = FSECTOR_WORLD_SIZE;
	
	// Entity Set
	std::set<Entity*> ents;

	// Scan Entities
	if ( zWorld->GetEntitiesInRect(sectorArea, ents) )
	{
		for( auto i = ents.begin(); i != ents.end(); ++i )
		{
			ProcessEntity(*i);
		}
	}
}

void MaterialSpawnManager::ProcessEntity(const Entity* entity)
{
	const MaterialSpawnSet* spawnSet = GetMaterialSpawnSet(entity->GetType());

	if ( spawnSet )
	{
		for( auto i = spawnSet->zMaterials.cbegin(); i != spawnSet->zMaterials.cend(); ++i )
		{
			float fRand = (float)rand() / (float)RAND_MAX;

			if ( i->chance > fRand )
			{
				// Spawn Material
				Material* new_item = new Material(*GetItemLookup()->GetMaterial(i->materialID));
				ItemActor* actor = new ItemActor(new_item);

				// Randomize Direction
				float dir = (float)rand() / (float)RAND_MAX * 3.14f;

				// Position
				Vector3 pos = entity->GetPosition() + Vector3(cos(dir), 0.0f, sin(dir)) * i->distance;
				try
				{
					pos = Vector3(pos.x, zWorld->CalcHeightAtWorldPos(pos.GetXZ())+0.1f, pos.z);
				}
				catch(...)
				{
				}
				actor->SetPosition(pos);

				// Add Actor
				zActorManager->AddActor(actor);
			}
		}
	}
}