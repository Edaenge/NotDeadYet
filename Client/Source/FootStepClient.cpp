#include "FootStepClient.h"
#include "ClientActorManager.h"


FootStepClient::FootStepClient(ClientActorManager* actorManager) :
	zActorManager(actorManager)
{
	if ( zActorManager )
	{
		zActorManager->AddObserver(this);
	}
}

FootStepClient::~FootStepClient()
{
	if ( zActorManager )
	{
		zActorManager->RemoveObserver(this);
	}
}

void OnEvent(Event* e)
{
	if ( ActorAddedEvent* AEE = dynamic_cast<ActorAddedEvent*>(e) )
	{

	}
}