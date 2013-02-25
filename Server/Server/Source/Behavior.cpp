#include <world/World.h>
#include "Behavior.h"
#include "Actor.h"


Behavior::Behavior( Actor* actor, World* world ) :
	zActor(actor),
	zWorld(world),
	zAnchor(0)
{
	zActor->AddObserver(this);

	if ( !zAnchor )
	{
		zAnchor = zWorld->CreateAnchor();
		zAnchor->position = zActor->GetPosition().GetXZ();
		zAnchor->radius = 32.0f;
	}
}

Behavior::~Behavior()
{
	if ( zAnchor && zWorld ) 
		zWorld->DeleteAnchor(zAnchor);

	zActor->RemoveObserver(this);
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
