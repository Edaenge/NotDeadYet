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
	// Print Positioning
	Vector2 printPos = actor->GetPosition().GetXZ();
	Vector2 forwardDir = actor->GetPosition().GetXZ() - zLastPositions[actor];
	forwardDir.Normalize();
	Vector2 rightDir = Vector3(0.0f, 1.0f, 0.0f).GetCrossProduct(Vector3(forwardDir.x, 0.0f, forwardDir.y)).GetXZ();
	rightDir.Normalize();

	// Print Texture
	const char* name = 0;

	// Make Prints
	if ( actor->GetModel() == "media/models/token_anims.fbx" )
	{
		unsigned int lastFoot = zLastFoot[actor];

		// Check Last Foot
		if ( !lastFoot )
		{
			name = "media/models/token_print_r.png";
			printPos += rightDir * 0.2f;
		}
		else
		{
			name = "media/models/token_print_l.png";
			printPos += Vector2(-rightDir.x, -rightDir.y) * 0.2f;
		}

		zLastFoot[actor] = (lastFoot+1) % 2;
	}

	if ( !name ) return;

	// Height Above Ground
	float groundHeight = 0.0f;
	try
	{
		groundHeight = zWorld->CalcHeightAtWorldPos(printPos);
	}
	catch(...)
	{
	}

	if ( zWorld->IsInside(printPos) )
	{
		// Ground Position
		if ( actor->GetPosition().y - groundHeight < 0.1f )
		{
			if ( name )
			{
				iDecal* data = zGraphics->CreateDecal(Vector3(printPos.x, groundHeight, printPos.y), name, Vector3(0.0f, -1.0f, 0.0f), Vector3(forwardDir.x, 0.0f, forwardDir.y));
				data->SetSize(0.5f);
				zFootSteps.insert(std::pair<unsigned int, iDecal*>(++zCurrentDecal, data));
			}
		}	
	}
}

void FootStepClient::OnEvent(Event* e)
{
	if ( ActorMovedEvent* AME = dynamic_cast<ActorMovedEvent*>(e) )
	{
		if ( (AME->zActor->GetPosition().GetXZ() - zLastPositions[AME->zActor]).GetLength() > 1.0f )
		{
			PlaceFootStep(AME->zActor);
			zLastPositions[AME->zActor] = AME->zActor->GetPosition().GetXZ();
		}
	}
	else if ( ActorChangedModelEvent* ACME = dynamic_cast<ActorChangedModelEvent*>(e) )
	{
		if ( ACME->zActor->GetModel() == "media/models/token_anims.fbx" )
		{
			zLastPositions[ACME->zActor] = Vector2(0.0f, 0.0f);
		}
		else
		{
			auto i = zLastPositions.find(ACME->zActor);
			if ( i != zLastPositions.cend() )
			{
				zLastPositions.erase(i);
			}
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
		// Remove from map
		auto i = zLastPositions.find(ARE->zActor);
		if ( i != zLastPositions.cend() )
		{
			AEE->zActor->RemoveObserver(this);
			zLastPositions.erase(i);
		}
	}
}