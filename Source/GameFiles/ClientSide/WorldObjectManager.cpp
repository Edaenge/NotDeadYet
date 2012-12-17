#include "GameFiles/ClientSide/WorldObjectManager.h"


WorldObjectManager::WorldObjectManager()
{

}

WorldObjectManager::~WorldObjectManager()
{
	for(auto x = this->zPlayers.begin(); x < this->zPlayers.end(); x++)
	{
		if((*x))
		{
			delete (*x);
			(*x) = 0;
		}
	}

	for(auto x = this->zAnimals.begin(); x < this->zAnimals.end(); x++)
	{
		if((*x))
		{
			delete (*x);
			(*x) = 0;
		}
	}

	for(auto x = this->zStaticObjects.begin(); x < this->zStaticObjects.end(); x++)
	{
		if((*x))
		{
			delete (*x);
			(*x) = 0;
		}
	}

	for(auto x = this->zDynamicObjects.begin(); x < this->zDynamicObjects.end(); x++)
	{
		if((*x))
		{
			delete (*x);
			(*x) = 0;
		}
	}
}

bool WorldObjectManager::AddObject(PlayerObject* player)
{
	if (player)
	{
		this->zPlayers.push_back(player);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(AnimalObject* animal)
{
	if (animal)
	{
		this->zAnimals.push_back(animal);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(StaticObject* staticObject)
{
	if (staticObject)
	{
		this->zStaticObjects.push_back(staticObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(DynamicObject* dynamicObject)
{
	if (dynamicObject)
	{
		this->zDynamicObjects.push_back(dynamicObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::RemoveObject(const unsigned int type, const unsigned int position)
{
	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		this->zPlayers.erase(this->zPlayers.begin() + position);
		break;
	case OBJECT_TYPE_ANIMAL:
		this->zAnimals.erase(this->zAnimals.begin() + position);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		this->zStaticObjects.erase(this->zStaticObjects.begin() + position);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		this->zDynamicObjects.erase(this->zDynamicObjects.begin() + position);
		break;
	default:
		return false;
		break;
	}
	return true;
}

std::vector<PlayerObject*> WorldObjectManager::GetPlayers()
{
	return this->zPlayers;
}

std::vector<AnimalObject*> WorldObjectManager::GetAnimals()
{
	return this->zAnimals;
}

std::vector<StaticObject*> WorldObjectManager::GetStaticObjects()
{
	return this->zStaticObjects;
}

std::vector<DynamicObject*> WorldObjectManager::GetDynamicObjects()
{
	return this->zDynamicObjects;
}

PlayerObject* WorldObjectManager::GetPlayer(const unsigned int pos)
{
	return this->zPlayers[pos];
}

AnimalObject* WorldObjectManager::GetAnimal(const unsigned int pos)
{
	return this->zAnimals[pos];
}

StaticObject* WorldObjectManager::GetStaticObject(const unsigned int pos)
{
	return this->zStaticObjects[pos];
}

DynamicObject* WorldObjectManager::GetDynamicObject( const unsigned int pos)
{
	return this->zDynamicObjects[pos];
}

int WorldObjectManager::SearchForObject(const unsigned int type, const unsigned int id)
{
	int position;
	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		position = this->SearchForPlayer(id);
		break;
	case OBJECT_TYPE_ANIMAL:
		position = this->SearchForAnimal(id);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		position = this->SearchForStaticObject(id);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		position = this->SearchForDynamicObject(id);
		break;
	default:
		position = -1;
		break;
	}
	return position;
}

int WorldObjectManager::SearchForPlayer(const unsigned int id)
{
	for (unsigned int i = 0; i < this->zPlayers.size(); i++)
	{
		if (this->zPlayers[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForStaticObject(const unsigned int id)
{
	for (unsigned int i = 0; i < this->zStaticObjects.size(); i++)
	{
		if (this->zStaticObjects[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForDynamicObject(const unsigned int id)
{
	for (unsigned int i = 0; i < this->zStaticObjects.size(); i++)
	{
		if (this->zDynamicObjects[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForAnimal(const unsigned int id)
{
	for (unsigned int i = 0; i < this->zAnimals.size(); i++)
	{
		if (this->zAnimals[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

bool WorldObjectManager::SearchAndRemove(const unsigned int type, const unsigned int id)
{
	int position;
	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		position = this->SearchForPlayer(id);
		this->zPlayers.erase(this->zPlayers.begin() + position);
		break;
	case OBJECT_TYPE_ANIMAL:
		position = this->SearchForAnimal(id);
		this->zAnimals.erase(this->zAnimals.begin() + position);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		position = this->SearchForStaticObject(id);
		this->zStaticObjects.erase(this->zStaticObjects.begin() + position);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		position = this->SearchForDynamicObject(id);
		this->zDynamicObjects.erase(this->zDynamicObjects.begin() + position);
		break;
	default:
		return false;
		break;
	}
	return true;
}

void WorldObjectManager::UpdateObjects( float deltaTime )
{
	for (auto x = this->zPlayers.begin(); x < this->zPlayers.end(); x++)
	{
		(*x)->Update(deltaTime);
	}

	for (auto x = this->zAnimals.begin(); x < this->zAnimals.end(); x++)
	{
		(*x)->Update(deltaTime);
	}

	for (auto x = this->zDynamicObjects.begin(); x < this->zDynamicObjects.end(); x++)
	{
		(*x)->Update(deltaTime);
	}
}
