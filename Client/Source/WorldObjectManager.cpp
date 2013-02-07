#include "WorldObjectManager.h"
#include "Safe.h"

WorldObjectManager::WorldObjectManager()
{

}

WorldObjectManager::~WorldObjectManager()
{
	for(auto x = this->zActors.begin(); x < this->zActors.end(); x++)
	{
		SAFE_DELETE((*x));
	}
}

Actor* WorldObjectManager::SearchAndGetActor(const unsigned int ID)
{
	int position = this->SearchForActor(ID);
	return this->zActors[position];
}

void WorldObjectManager::UpdateObjects( float deltaTime )
{
	for (auto x = this->zUpdates.begin(); x < this->zUpdates.end(); x++)
	{
		this->SearchAndGetActor((*x).GetID());
	}
}

Actor* WorldObjectManager::GetActor( const int Index )
{
	if ((unsigned int)Index < this->zActors.size())
		return this->zActors[Index];

	return NULL;
}

int WorldObjectManager::SearchForActor( const unsigned int ID )
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

bool WorldObjectManager::RemoveActor( const int Index )
{
	if ((unsigned int)Index < this->zActors.size())
	{
		Actor* worldObject = this->zActors[Index];
		this->zActors.erase(this->zActors.begin() + Index);
		SAFE_DELETE(worldObject);
		return true;
	}
}

bool WorldObjectManager::AddActor(Actor* actor)
{
	if (actor)
	{
		this->zActors.push_back(actor);
		return true;
	}
	return false;
}

void WorldObjectManager::AddUpdate( Updates update )
{
	this->zUpdates.push_back(update);
}