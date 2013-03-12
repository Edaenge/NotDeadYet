#include <world/World.h>
#include "Behavior.h"
#include "Actor.h"

using namespace std::chrono;


Behavior::Behavior( Actor* actor, World* world ) :
	zActor(actor),
	zWorld(world),
	zAnchor(0),
	zAwakeTime(time_point_cast<microseconds>(system_clock::now()))
{
	zActor->AddObserver(this);

	if ( !zAnchor )
	{
		zAnchor = zWorld->CreateAnchor();
		zAnchor->position = zActor->GetPosition().GetXZ();
		zAnchor->radius = 32.0f;
	}
	this->zRemoveBehavior = false;
}

Behavior::~Behavior()
{
	if ( zAnchor && zWorld ) 
		zWorld->DeleteAnchor(zAnchor);

	if (zActor)
		zActor->RemoveObserver(this);
}

void Behavior::Sleep(float time)
{
	zAwakeTime = time_point_cast<std::chrono::microseconds>(system_clock::now()) + std::chrono::microseconds((long long)(time * 1000000.0f));
}

bool Behavior::IsAwake() const
{
	return time_point_cast<microseconds>(system_clock::now()) >= zAwakeTime;
}

void Behavior::OnEvent(Event* e)
{
	if ( ActorPositionEvent* APE = dynamic_cast<ActorPositionEvent*>(e) )
	{
		zAnchor->position = APE->zActor->GetPosition().GetXZ();
	}
	else if ( ActorDeleteEvent* ADE = dynamic_cast<ActorDeleteEvent*>(e) )
	{
		zWorld->DeleteAnchor(zAnchor);
		zActor = NULL;
	}
}

bool Behavior::Update( float )
{
	return ( zActor == NULL );
}
