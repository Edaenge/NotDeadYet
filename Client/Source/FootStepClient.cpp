#include "FootStepClient.h"
#include "ClientActorManager.h"
#include "Actor.h"
#include "Graphics.h"


FootStepClient::FootStepClient(GraphicsEngine* graphics, ClientActorManager* actorManager, World* world) :
	zGraphics(graphics),
	zActorManager(actorManager),
	zWorld(world)
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

	// Delete Steps
	for( auto i = zFootSteps.cbegin(); i != zFootSteps.cend(); ++i )
	{
		zGraphics->DeleteDecal(i->second);
	}
}

void FootStepClient::PurgeSteps()
{
	Vector3 camPos = zGraphics->GetCamera()->GetPosition();
	float clipRange = zGraphics->GetEngineParameters().FarClip;

	for ( auto i = zFootSteps.cbegin(); i != zFootSteps.cend(); )
	{
		if ( (camPos - (i->second)->GetPosition()).GetLength() >= clipRange )
		{
			zGraphics->DeleteDecal(i->second);
			i = zFootSteps.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void FootStepClient::Update()
{
	if ( (zGraphics->GetCamera()->GetPosition() - zLastPurgePos).GetLength() > 1.0f )
	{
		PurgeSteps();
		zLastPurgePos = zGraphics->GetCamera()->GetPosition();
	}

	while ( zFootSteps.size() > 100 )
	{
		zGraphics->DeleteDecal(zFootSteps.cbegin()->second);
		zFootSteps.erase(zFootSteps.begin());
	}
}

void FootStepClient::PlaceFootStep( Actor* actor )
{
	Vector2 forwardDir = actor->GetPosition().GetXZ() - zLastPositions[actor];
	forwardDir.Normalize();

	// Height Above Ground
	float groundHeight = 0.0f;
	try
	{
		groundHeight = zWorld->CalcHeightAtWorldPos(actor->GetPosition().GetXZ());
	}
	catch(...)
	{
	}

	// Ground Position
	if ( actor->GetPosition().y - groundHeight < 0.1f )
	{
		iDecal* data = zGraphics->CreateDecal(Vector3(actor->GetPosition().x, groundHeight, actor->GetPosition().z), "media/models/shoeprint.png", Vector3(0.0f, -1.0f, 0.0f), Vector3(forwardDir.x, 0.0f, forwardDir.y));
		data->SetSize(0.5f);
		zFootSteps.insert(std::pair<unsigned int, iDecal*>(++zCurrentDecal, data));
	}	
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