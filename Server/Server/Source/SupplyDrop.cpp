#include "SupplyDrop.h"
#include <world/World.h>
#include "ActorManager.h"
#include "SupplyActor.h"
#include <MaloW.h>
#include <PhysicsEngine.h>
#include "Physics.h"
#include "SupplyDropBehavior.h"
#include <ctime>

SupplyDrop::SupplyDrop( ActorManager* actorManager, World* world, SoundHandler* soundHandler )
	:zWorld(world), zActorManager(actorManager), zSoundHandler(soundHandler)
{
	srand( (unsigned int) time(0) );

	this->zActorManager = actorManager;
	this->zPhyEngine = GetPhysics();
}

SupplyDrop::~SupplyDrop()
{

}

bool SupplyDrop::SpawnSupplyDrop( Vector2& landPoint, std::set<Item*>& items, float height )
{
	unsigned int weight = CalculateTotalWeight(items);
	return SpawnSupplyDrop(landPoint, items, weight, height);
}

bool SupplyDrop::SpawnSupplyDrop( Vector2& landPoint, std::set<Item*>& items, const unsigned int itemCapacity, float height )
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
	if( !phyOBj && !zWorld->IsInside(landPoint) )
	{
		delete spActor, spActor = NULL;
		this->zPhyEngine->DeletePhysicsObject(phyOBj);

		return false;
	}

	Vector3 position = Vector3(landPoint.x, height, landPoint.y);

	//Set Values
	spActor->SetPhysicsObject(phyOBj);
	spActor->SetPosition(position, false);
	spActor->SetScale(spActor->GetScale(), false);

	//Create Behavior
	SupplyDropBehavior* behavior = new SupplyDropBehavior(spActor, zWorld, landPoint, zSoundHandler);

	zActorManager->AddBehavior(behavior);
	zActorManager->AddActor(spActor);
	
	return true;
}

bool SupplyDrop::SpawnAirbornSupplyDrop( Vector2& landPoint, float height, std::set<Item*>& items )
{
	unsigned int weight = CalculateTotalWeight(items);
	return SpawnAirbornSupplyDrop(landPoint, height, items, weight);
}

bool SupplyDrop::SpawnAirbornSupplyDrop( Vector2& landPoint, float height, std::set<Item*>& items, const unsigned int itemCapacity )
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
	if( !phyOBj && !zWorld->IsInside(landPoint) )
	{
		delete spActor, spActor = NULL;
		this->zPhyEngine->DeletePhysicsObject(phyOBj);

		return false;
	}

	Vector2 position2D = landPoint;
	Vector3 position3D = Vector3(position2D.x, height, position2D.y);

	//Set Values
	spActor->SetPhysicsObject(phyOBj);
	spActor->SetPosition(position3D, false);
	spActor->SetScale(spActor->GetScale(), false);

	//Create parachute
	Actor* paraActor = new Actor();
	Vector3 pos = position3D;
	paraActor->SetPosition(pos, false);
	paraActor->SetModel(PARACHUTE_FILE_NAME);
	spActor->AttachParachute(paraActor);

	//Create behavior
	SupplyDropBehavior* behavior = new SupplyDropBehavior(spActor, zWorld, landPoint, zSoundHandler);

	zActorManager->AddActor(spActor);
	zActorManager->AddActor(paraActor);
	zActorManager->AddBehavior(behavior);

	return true;
}

unsigned int SupplyDrop::CalculateTotalWeight( std::set<Item*>& items ) const
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

	return weight;
}

Vector2 SupplyDrop::RandomizePos( const Vector2& landpos )
{
	float radius = (float) (rand() % 1+5);
	float distanceInAxes = sqrtf(powf(radius, 2.0f) * 0.5f);

	Vector2 one = Vector2(distanceInAxes, distanceInAxes);
	Vector2 two = Vector2(distanceInAxes, -distanceInAxes);
	Vector2 three = Vector2(-distanceInAxes, -distanceInAxes);
	Vector2 four = Vector2(-distanceInAxes, distanceInAxes);

	int point = rand() % 1+4;

	Vector2 temp = landpos;

	switch (point)
	{
	case 1:
		temp += one;
		break;
	case 2:
		temp += two;
		break;
	case 3:
		temp += three;
		break;
	case 4:
		temp += four;
		break;
	}

	return temp;
}