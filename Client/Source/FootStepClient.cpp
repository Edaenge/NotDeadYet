#include "FootStepClient.h"
#include "ClientActorManager.h"
#include "Actor.h"
#include "Graphics.h"


FootStepClient::FootStepClient(GraphicsEngine* graphics, ClientActorManager* actorManager) :
	zGraphics(graphics),
	zActorManager(actorManager)
{
	if ( zActorManager )
	{
		zActorManager->AddObserver(this);
	}
}

FootStepClient::~FootStepClient()
{
	for( auto i = zLastPositions.cbegin(); i != zLastPositions.cend(); ++i )
	{
		i->first->RemoveObserver(this);
	}

	if ( zActorManager )
	{
		zActorManager->RemoveObserver(this);
	}
}

void FootStepClient::PlaceFootStep( Actor* actor )
{
	iDecal* data = zGraphics->CreateDecal(actor->GetPosition(), "Red.png", Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	zFootSteps.insert(data);
}

void FootStepClient::OnEvent(Event* e)
{
	if ( ActorMovedEvent* AME = dynamic_cast<ActorMovedEvent*>(e) )
	{
		if ( (AME->zActor->GetPosition().GetXZ() - zLastPositions[AME->zActor]).GetLength() > 0.5f )
		{
			PlaceFootStep(AME->zActor);
			zLastPositions[AME->zActor] = AME->zActor->GetPosition().GetXZ();
		}
	}
	else if ( ActorAddedEvent* AEE = dynamic_cast<ActorAddedEvent*>(e) )
	{
		if ( AEE->zActor->GetModel() == "media/models/token_anims.fbx" )
		{
			zLastPositions[AEE->zActor] = Vector2(0.0f, 0.0f);
			AEE->zActor->AddObserver(this);
		}
	}
	else if ( ActorRemovedEvent* ARE = dynamic_cast<ActorRemovedEvent*>(e) )
	{
		// Stop observing
		AEE->zActor->RemoveObserver(this);
		
		// Remove from map
		auto i = zLastPositions.find(ARE->zActor);
		if ( i != zLastPositions.cend() )
		{
			zLastPositions.erase(i);
		}
	}
}