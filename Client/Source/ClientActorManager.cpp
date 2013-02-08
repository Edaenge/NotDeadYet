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
	for(auto x = this->zActors.begin(); x < this->zActors.end(); x++)
	{
		SAFE_DELETE((*x));
	}
}

Actor* ClientActorManager::SearchAndGetActor(const unsigned int ID)
{
	int position = this->SearchForActor(ID);
	if ( position < 0 ) return 0;
	return this->zActors[position];
}

void ClientActorManager::UpdateObjects( float deltaTime, unsigned int clientID )
{
	float t = GetInterpolationType(deltaTime, IT_SMOOTH_STEP);

	auto it_Update = this->zUpdates.begin();
	while( it_Update != this->zUpdates.end() )
	{
		Actor* actor = this->SearchAndGetActor((*it_Update)->GetID());

		if (actor)
		{
			if ((*it_Update)->HasPositionChanged())
			{
				Vector3 position = this->InterpolatePosition(actor->GetPosition(), (*it_Update)->GetPosition(), t);
				if((*it_Update)->GetID() == clientID)
				{
					GetSounds()->PlaySounds("Media/Sound/Walk.wav", position);
					GetGraphics()->GetCamera()->SetPosition(position + Vector3(0.0f, 2.0f, 0.0f));
				}
				actor->SetPosition(position);
				(*it_Update)->ComparePosition(position);
			}
			if((*it_Update)->GetID() != clientID)
			{
				if ((*it_Update)->HasRotationChanged())
				{
					Vector4 rotation = this->InterpolateRotation(actor->GetRotation(), (*it_Update)->GetRotation(), t);
					actor->SetRotation(rotation);
					(*it_Update)->CompareRotation(rotation);
				}
			}
			else
			{
				(*it_Update)->SetRotationChanged(false);
			}
			if ((*it_Update)->HasStateChanged())
			{
				actor->SetState((*it_Update)->GetState());
				(*it_Update)->SetStateChange(false);
			}
			
			if (!(*it_Update)->HasPositionChanged() && !(*it_Update)->HasRotationChanged() && !(*it_Update)->HasStateChanged())
			{
				it_Update = zUpdates.erase(it_Update);
			}
			else
			{
				++it_Update;
			}
		}
	}
}

Actor* ClientActorManager::GetActor( const int Index )
{
	if ((unsigned int)Index < this->zActors.size())
		return this->zActors[Index];

	return NULL;
}

int ClientActorManager::SearchForActor( const unsigned int ID )
{
	for (unsigned int i = 0; i < this->zActors.size(); i++)
	{
		if (this->zActors[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

bool ClientActorManager::RemoveActor( const int Index )
{
	if ((unsigned int)Index < this->zActors.size())
	{
		Actor* worldObject = this->zActors[Index];
		this->zActors.erase(this->zActors.begin() + Index);
		SAFE_DELETE(worldObject);
		return true;
	}
}

bool ClientActorManager::AddActor(Actor* actor)
{
	if (actor)
	{
		this->zActors.push_back(actor);
		return true;
	}
	return false;
}

void ClientActorManager::AddUpdate( Updates* update )
{
	this->zUpdates.push_back(update);
}

int ClientActorManager::SearchForUpdate( const unsigned int ID )
{
	for (unsigned int i = 0; i < this->zUpdates.size(); i++)
	{
		if (this->zUpdates[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

Updates* ClientActorManager::GetUpdate( const int Index )
{
	if ((unsigned int)Index < this->zUpdates.size())
		return this->zUpdates[Index];

	return NULL;
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