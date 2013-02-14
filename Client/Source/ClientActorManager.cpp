#include "ClientActorManager.h"
#include "Safe.h"
#include "Graphics.h"
#include "Sounds.h"
#define PI 3.14159265358979323846f

ClientActorManager::ClientActorManager()
{
	zInterpolationVelocity = 500.0f;
}

ClientActorManager::~ClientActorManager()
{
	for(auto it = this->zActors.begin(); it != this->zActors.end(); it++)
	{
		Actor* temp = it->second;

		SAFE_DELETE(temp);

		it = this->zActors.erase(it);
	}
}

void ClientActorManager::UpdateObjects( float deltaTime, unsigned int clientID )
{
	float t = GetInterpolationType(deltaTime, IT_SMOOTH_STEP);
	static GraphicsEngine* gEng = GetGraphics();
	auto it_Update = this->zUpdates.begin();
	while( it_Update != this->zUpdates.end() )
	{
		Updates* update = it_Update->second;
		Actor* actor = this->GetActor(update->GetID());

		if (actor)
		{
			if (update->HasPositionChanged())
			{
				Vector3 position;
				if(update->GetID() == clientID)
				{
					position = this->InterpolatePosition(actor->GetPosition(), update->GetPosition(), t);
					//if ( rand()%10000 == 0 ) GetSounds()->PlaySounds("Media/Sound/Walk.wav", position);
					actor->SetPosition(position);
					GetGraphics()->GetCamera()->SetPosition(position + Vector3(0.0f, 2.5f, 0.0f));
					
				}
				else 
				{
					position = this->InterpolatePosition(actor->GetPosition(), update->GetPosition(), t);
					actor->SetPosition(position);
				}
				
				update->ComparePosition(position);
			}
			//if((*it_Update)->GetID() != clientID)
			//{
			//	if ((*it_Update)->HasRotationChanged())
			//	{
			//		Vector4 rotation = this->InterpolateRotation(actor->GetRotation(), (*it_Update)->GetRotation(), t);
			//		actor->SetRotation(rotation);
			//		(*it_Update)->CompareRotation(rotation);
			//	}
			//}
			//else
			//{
			//	(*it_Update)->SetRotationChanged(false);
			//}
			//if ((*it_Update)->HasStateChanged())
			//{
			//	actor->SetState((*it_Update)->GetState());
			//	(*it_Update)->SetStateChange(false);
			//}
			
			if (!update->HasPositionChanged() )//&& !(*it_Update)->HasRotationChanged() && !(*it_Update)->HasStateChanged())
			{
				Updates* temp = update; 
				it_Update = zUpdates.erase(it_Update);
				SAFE_DELETE(temp);
			}
			else
			{
				it_Update++;
			}
		}
		else
		{
			Updates* temp = update; 
			it_Update = zUpdates.erase(it_Update);
			SAFE_DELETE(temp);
		}
	}
}

std::map<unsigned int, Actor*> ClientActorManager::GetActors()
{
	return this->zActors;
}

Actor* ClientActorManager::GetActor( unsigned int ID )
{
	auto actorIterator = this->zActors.find(ID);

	if (actorIterator != this->zActors.end())
		return actorIterator->second;

	return NULL;
}

Updates* ClientActorManager::GetUpdate(const int ID)
{
	auto updateIterator = this->zUpdates.find(ID);

	if (updateIterator != this->zUpdates.end())
		return updateIterator->second;

	return NULL;
	
}

void ClientActorManager::RemoveActor( const unsigned int ID )
{
	auto actorIterator = this->zActors.find(ID);

	Actor* actor = actorIterator->second;

	SAFE_DELETE(actor);

	this->zActors.erase(actorIterator);
}

bool ClientActorManager::AddActor(Actor* actor)
{
	if (actor)
	{
		this->zActors[actor->GetID()] = actor;
		return true;
	}
	return false;
}

void ClientActorManager::AddUpdate( Updates* update )
{
	if (update)
		this->zUpdates[update->GetID()] = update;
}

float ClientActorManager::GetInterpolationType(const float deltaTime, const unsigned int type)
{
	float t;
	switch (type)
	{
	case IT_LINEAR:
		t = deltaTime;
		break;
	case IT_COSINE:
		t = (-cos(PI * deltaTime) * 0.5f) + 0.5f;
		break;
	case IT_ACCELERATION:
		t = pow(deltaTime, 2);
		break;
	case IT_SMOOTH_STEP:
		t = pow(deltaTime, 2) * (3 - 2 * deltaTime);
		break;
	case IT_DECELERATION:
		t = (1 - pow(1 - deltaTime, 2));
		break;
	default:
		t = deltaTime;
		break;
	}

	return t;
}

Vector3 ClientActorManager::InterpolatePosition(const Vector3& currentPosition, const Vector3& newPosition, float t )
{
	float oldlength = (currentPosition - newPosition).GetLength();

	if (oldlength > 200.0f)
		return newPosition;

	Vector3 returnPosition = currentPosition + (newPosition - currentPosition) * t * zInterpolationVelocity;

	float newLength = (returnPosition - newPosition).GetLength();

	if (newLength > oldlength)
		returnPosition = newPosition;


	return returnPosition;
}

Vector4 ClientActorManager::InterpolateRotation( const Vector4& currentRotation, const Vector4& newRotation, float t )
{
	float oldlength = (currentRotation - newRotation).GetLength();

	Vector4 returnRotation = currentRotation + (newRotation - currentRotation) * t * zInterpolationVelocity;

	float newLength = (returnRotation - newRotation).GetLength();

	if (newLength > oldlength)
		returnRotation = newRotation;

	return returnRotation;
}