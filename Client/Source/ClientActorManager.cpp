#include "ClientActorManager.h"
#include "Safe.h"
#include "Graphics.h"

#define PI 3.14159265358979323846f
#define MAXFOOTSTEPS 6
#define NROFGRASSTEX 3
#define NROFDIRTTEX 3
ClientActorManager::ClientActorManager()
{
	AudioManager* am = AudioManager::GetInstance();

	/*this->zFootStepGrass = new IEventHandle*[MAXFOOTSTEPS];
	for(int i = 0; i < MAXFOOTSTEPS; i++)
		am->GetEventHandle(EVENTID_NOTDEADYET_WALK_GRASS, this->zFootStepGrass[i]);

	this->zFootStepDirt = new IEventHandle*[MAXFOOTSTEPS];
	for(int i = 0; i < MAXFOOTSTEPS; i++)
		am->GetEventHandle(EVENTID_NOTDEADYET_WALK_DIRT, this->zFootStepDirt[i]);*/

	this->zFootStepsOnGrass = new Sound*[MAXFOOTSTEPS];
	for(int i = 0; i < MAXFOOTSTEPS; i++)
		zFootStepsOnGrass[i] = new Sound(EVENTID_NOTDEADYET_WALK_GRASS);

	this->zFootStepsOnDirt = new Sound*[MAXFOOTSTEPS];
	for(int i = 0; i < MAXFOOTSTEPS; i++)
		zFootStepsOnDirt[i] = new Sound(EVENTID_NOTDEADYET_WALK_DIRT);

	this->zInterpolationVelocity = 1.0f;
	this->zUpdatesPerSec = 1;
	this->zLatency = 1;
}

ClientActorManager::~ClientActorManager()
{
	auto it_zActors_end = this->zActors.end();
	for(auto it = this->zActors.begin(); it != it_zActors_end; it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	this->zActors.clear();
	auto it_zUpdates_end = this->zUpdates.end();
	for(auto it = this->zUpdates.begin(); it != it_zUpdates_end; it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	this->zUpdates.clear();
	/*// Delete grass footsteps
	for(int i = 0; i < MAXFOOTSTEPS; i++)
	{
		this->zFootStepGrass[i]->Release();
		delete this->zFootStepGrass[i];
		this->zFootStepGrass[i] = NULL;
	}

	delete this->zFootStepGrass;
	this->zFootStepGrass = NULL;

	//Delete dirt footsteps
	for(int i = 0; i < MAXFOOTSTEPS; i++)
	{
		this->zFootStepDirt[i]->Release();
		delete this->zFootStepDirt[i];
		this->zFootStepDirt[i] = NULL;
	}

	delete this->zFootStepDirt;
	this->zFootStepDirt = NULL;*/

	for(int i = 0; i < MAXFOOTSTEPS; i++)
	{
		delete this->zFootStepsOnGrass[i];
		this->zFootStepsOnGrass[i] = NULL;
	}
	delete this->zFootStepsOnGrass;

	for(int i = 0; i < MAXFOOTSTEPS; i++)
	{
		delete this->zFootStepsOnDirt[i];
		this->zFootStepsOnDirt[i] = NULL;
	}
	delete this->zFootStepsOnDirt;
}

void ClientActorManager::UpdateObjects( float deltaTime, unsigned int clientID, World* world )
{
	for(int i = 0; i < MAXFOOTSTEPS; i++)
	{
		this->zFootStepsOnDirt[i]->Update(deltaTime);
		this->zFootStepsOnGrass[i]->Update(deltaTime);
	}

	//std::vector<Actor*> actors;
	float latency = 1.0f / (float)this->zLatency;
	float server_DeltaTime = 1.0f / (float)this->zUpdatesPerSec;
	float t = deltaTime + latency;//GetInterpolationType(deltaTime, IT_SMOOTH_STEP);
	static GraphicsEngine* gEng = GetGraphics();
	int stepsPlayedThisUpdate = 1;

	Vector3 position;
	auto it_Update = this->zUpdates.begin();
	while( it_Update != this->zUpdates.end() )
	{
		Updates* update = it_Update->second;
		Actor* actor = this->GetActor(update->GetID());
		position = Vector3(0.0f, 0.0f, 0.0f);

		if (actor)
		{
			if (update->HasStateChanged())
			{
				auto actorIterator = this->zState.find(actor);
				if (actorIterator != this->zState.end())
					actorIterator->second = update->GetState();

				update->SetStateChange(false);
			}
			if (update->HasPositionChanged())
			{
				Vector3 oldPosition;
				if(update->GetID() == clientID)
				{
					position = this->InterpolatePosition(gEng->GetCamera()->GetPosition() - this->zCameraOffset, update->GetPosition(), t);
					AudioManager::GetInstance()->SetPlayerPosition(&ConvertToFmodVector(position), &ConvertToFmodVector(gEng->GetCamera()->GetForward()), &ConvertToFmodVector(gEng->GetCamera()->GetUpVector()));
					if(actor->GetModel() != "Media/Models/ghost.obj")
					{
						int mostUsedTex = this->GetMostUsedTexOnPos(position, world);
						if(mostUsedTex == 0)
						{
							/*this->zFootStepGrass[MAXFOOTSTEPS-1]->Setposition(&ConvertToFmodVector(position));
							this->zFootStepGrass[MAXFOOTSTEPS-1]->Play();*/
							this->zFootStepsOnGrass[MAXFOOTSTEPS-1]->SetPosition(position);
							this->zFootStepsOnGrass[MAXFOOTSTEPS-1]->Play();
						}
						else
						{
							this->zFootStepsOnDirt[MAXFOOTSTEPS-1]->SetPosition(position);
							this->zFootStepsOnDirt[MAXFOOTSTEPS-1]->Play();
						}
					}

					gEng->GetCamera()->SetPosition(position + this->zCameraOffset);
				}
				else 
				{
					position = this->InterpolatePosition(actor->GetPosition(), update->GetPosition(), t);
					actor->SetPosition(position);
					if(actor->GetModel() != "Media/Models/ghost.obj")
					{
						if(stepsPlayedThisUpdate < MAXFOOTSTEPS)
						{
							if(this->GetMostUsedTexOnPos(position, world) == 0)
							{
								/*this->zFootStepGrass[stepsPlayedThisUpdate]->Setposition(&ConvertToFmodVector(position));
								this->zFootStepGrass[stepsPlayedThisUpdate]->Play();*/
								this->zFootStepsOnGrass[stepsPlayedThisUpdate]->SetPosition(position);
								this->zFootStepsOnGrass[stepsPlayedThisUpdate]->Play();
							}
							else
							{
								/*this->zFootStepDirt[stepsPlayedThisUpdate]->Setposition(&ConvertToFmodVector(position));
								this->zFootStepDirt[stepsPlayedThisUpdate]->Play();*/
								this->zFootStepsOnDirt[stepsPlayedThisUpdate]->SetPosition(position);
								this->zFootStepsOnDirt[stepsPlayedThisUpdate]->Play();
							}
						}
					}

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
			if (!update->HasPositionChanged() )//&& !(*it_Update)->HasRotationChanged() && !(*it_Update)->HasStateChanged())
			{
				Updates* temp = update; 
				it_Update = zUpdates.erase(it_Update);
				SAFE_DELETE(temp);
			}
			else
			{
				it_Update++;
				stepsPlayedThisUpdate++;
				if(stepsPlayedThisUpdate >= MAXFOOTSTEPS)
					stepsPlayedThisUpdate = 0;
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

	auto actorStateIterator = this->zState.find(actor);
	if (actorStateIterator != this->zState.end())
		this->zState.erase(actorStateIterator);

	SAFE_DELETE(actor);

	this->zActors.erase(actorIterator);
}

void ClientActorManager::AddActorState( Actor* actor, unsigned int state )
{
	if (actor)
		this->zState[actor] = state;
}

unsigned int ClientActorManager::GetState( Actor* actor )
{
	if (actor)
	{
		auto stateIterator = this->zState.find(actor);
		if (stateIterator != this->zState.end())
			return stateIterator->second;
	}	

	return 500;
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

	//float totalDelay = this->zLatency + this->zUpdatesPerSec;
	
	//if (totalDelay < 10.0f)
	//	totalDelay = this->zInterpolationVelocity;

	//float step = 100 / this->zLatency * t;
	//float delta = 1 - step * t;
	Vector3 diff = newPosition - currentPosition;
	Vector3 returnPosition = currentPosition + diff * t * this->zInterpolationVelocity;

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

void ClientActorManager::ClearAll()
{
	auto it_zActors_end = this->zActors.end();
	for (auto it = this->zActors.begin(); it != it_zActors_end; it++)
	{
		auto ret = zUpdates.find(it->first);

		if( ret != zUpdates.end() )
			SAFE_DELETE(ret->second);
		
		SAFE_DELETE(it->second);
	}

	zActors.clear();
	zUpdates.clear();
	zState.clear();

}

FMOD_VECTOR ClientActorManager::ConvertToFmodVector( const Vector3& v ) const
{
	FMOD_VECTOR temp;
	temp.x = v.x;
	temp.y = v.y;
	temp.z = v.z;
	return temp;
}

int ClientActorManager::GetMostUsedTexOnPos( Vector3 pos, World* world )
{
	float mostGrassTex = 0.0f;
	float mostDirtTex = 0.0;
	float texGrass[NROFGRASSTEX];
	texGrass[0] = world->GetAmountOfTexture(pos.GetXZ(), "01_v02-Moss.png");
	texGrass[1] = world->GetAmountOfTexture(pos.GetXZ(), "06_v01-MossDark.png");
	texGrass[2] = world->GetAmountOfTexture(pos.GetXZ(), "07_v01-MossLight.png");

	float texDirt[NROFDIRTTEX];
	texDirt[0] = world->GetAmountOfTexture(pos.GetXZ(), "05_v01-Sandpng.png");
	texDirt[1] = world->GetAmountOfTexture(pos.GetXZ(), "04_v02-Gravel.png");
	texDirt[2] = world->GetAmountOfTexture(pos.GetXZ(), "03_v02-Mix.png");

	for(int i = 0; i < NROFGRASSTEX; i++)
	{
		if(texGrass[i] > mostGrassTex)
			mostGrassTex = texGrass[i];
	}

	for(int i = 0; i < NROFDIRTTEX; i++)
	{
		if(texDirt[i] > mostGrassTex)
			mostDirtTex = texDirt[i];
	}

	if(mostDirtTex > mostGrassTex)
		return 1;
	
	return 0;

}
