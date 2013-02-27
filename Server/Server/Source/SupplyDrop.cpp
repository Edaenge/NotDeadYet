#include "SupplyDrop.h"
#include <world/World.h>
#include "ActorManager.h"
#include "SupplyActor.h"
#include <MaloW.h>
#include <PhysicsEngine.h>

//MODEL NAME
static const std::string SUPPLY_ACTOR_FILENAME = "Media/Models/Cube_4.obj";

SupplyDrop::SupplyDrop( ActorManager* actorManager, World* world, PhysicsEngine* engine )
	:zWorld(world), zActorManager(actorManager), zPhyEngine(engine)
{
	this->zActorManager = actorManager;
	this->AddObserver( this->zActorManager );
}

SupplyDrop::~SupplyDrop()
{

}

bool SupplyDrop::SpawnSupplyDrop( Vector3& pos, std::set<Item*>& items )
{
	unsigned int weight = 0;
	
	//Calculate total weight
	for( auto it = items.begin(); it != items.end(); it++ )
	{
		if( (*it) == NULL )
		{
			it = items.erase(it);
			continue;
		}

		weight += (*it)->GetWeight() * (*it)->GetStackSize();
	}
	
	return SpawnSupplyDrop(pos, items, weight);
}

bool SupplyDrop::SpawnSupplyDrop( Vector3& pos, std::set<Item*>& items, const unsigned int itemCapacity )
{
	SupplyActor* spActor = new SupplyActor(itemCapacity);

	for( auto it = items.begin(); it != items.end(); it++ )
	{
		if( !spActor->AddItem(*it) )
		{
			MaloW::Debug("In SupplyDrop, could not add item in SpawnSupplyDrop.");
			Item* temp = (*it);
			it = items.erase(it);
			
			SAFE_DELETE(temp);
		}
	}
	
	//Create physic obj.
	PhysicsObject* phyOBj = zPhyEngine->CreatePhysicsObject(SUPPLY_ACTOR_FILENAME);

	//Check if physObj is not null and if the pos given is inside the world.
	if( !phyOBj && !zWorld->IsInside(pos.GetXZ()) )
	{
		delete spActor, spActor = NULL;
		this->zPhyEngine->DeletePhysicsObject(phyOBj);

		return false;
	}

	//We want it on the ground, calc the height
	float yVale = zWorld->CalcHeightAtWorldPos( pos.GetXZ() );
	pos.y = yVale;

	//Set Values
	spActor->SetPhysicsObject(phyOBj);
	spActor->SetPosition(pos);
	spActor->SetScale(spActor->GetScale());

	//Notify
	//NotifyObservers();

	return true;
}

bool SupplyDrop::SpawnAirbornSupplyDrop( Vector3& pos, std::set<Item*>& items )
{

	return false;
}

bool SupplyDrop::SpawnAirbornSupplyDrop( Vector3& pos, std::set<Item*>& items, const unsigned int itemCapacity )
{

	return false;
}
