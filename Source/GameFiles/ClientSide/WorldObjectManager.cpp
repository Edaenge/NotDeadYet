#include "GameFiles/ClientSide/WorldObjectManager.h"


WorldObjectManager::WorldObjectManager()
{
	this->zTerrain = NULL;
}

WorldObjectManager::~WorldObjectManager()
{
	for(auto x = this->zPlayerObjects.begin(); x < this->zPlayerObjects.end(); x++)
	{
		if((*x))
		{
			delete (*x);
			(*x) = 0;
		}
	}

	for(auto x = this->zAnimalObjects.begin(); x < this->zAnimalObjects.end(); x++)
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
	this->zMapObjects.erase(this->zMapObjects.begin(), this->zMapObjects.end());

	if (this->zTerrain)
	{
		//delete this->zTerrain;
		this->zTerrain = NULL;
	}
}

bool WorldObjectManager::AddObject(PlayerObject* player)
{
	if (player)
	{
		this->zPlayerObjects.push_back(player);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(AnimalObject* animal)
{
	if (animal)
	{
		this->zAnimalObjects.push_back(animal);
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
	if (type == OBJECT_TYPE_PLAYER)
	{
		if (position < this->zPlayerObjects.size())
		{
			PlayerObject* playerObj = this->zPlayerObjects[position];
			this->zPlayerObjects.erase(this->zPlayerObjects.begin() + position);
			if (playerObj)
			{
				delete playerObj;
				playerObj = NULL;
			}
			return true;
		}
	}
	if (type == OBJECT_TYPE_ANIMAL)
	{
		if (position < this->zAnimalObjects.size())
		{
			AnimalObject* animalObj = this->zAnimalObjects[position];
			this->zAnimalObjects.erase(this->zAnimalObjects.begin() + position);
			if (animalObj)
			{
				delete animalObj;
				animalObj = NULL;
			}
			return true;
		}
	}
	if (type == OBJECT_TYPE_STATIC_OBJECT)
	{
		if (position < this->zStaticObjects.size())
		{
			StaticObject* staticObj = this->zStaticObjects[position];
			this->zStaticObjects.erase(this->zStaticObjects.begin() + position);
			if (staticObj)
			{
				delete staticObj;
				staticObj = NULL;
			}
			return true;
		}
	}
	if (type == OBJECT_TYPE_DYNAMIC_OBJECT)
	{
		if (position < this->zDynamicObjects.size())
		{
			DynamicObject* dynamicObj = this->zDynamicObjects[position];
			this->zDynamicObjects.erase(this->zDynamicObjects.begin() + position);
			if (dynamicObj)
			{
				delete dynamicObj;
				dynamicObj = NULL;
			}
			return true;
		}
	}
	return false;
}

PlayerObject* WorldObjectManager::GetPlayerObject(const unsigned int pos)
{
	return this->zPlayerObjects[pos];
}

AnimalObject* WorldObjectManager::GetAnimalObject(const unsigned int pos)
{
	return this->zAnimalObjects[pos];
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
		position = this->SearchForPlayerObject(id);
		break;
	case OBJECT_TYPE_ANIMAL:
		position = this->SearchForAnimalObject(id);
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

int WorldObjectManager::SearchForPlayerObject(const unsigned int id)
{
	for (unsigned int i = 0; i < this->zPlayerObjects.size(); i++)
	{
		if (this->zPlayerObjects[i]->GetID() == id)
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

int WorldObjectManager::SearchForAnimalObject(const unsigned int id)
{
	for (unsigned int i = 0; i < this->zAnimalObjects.size(); i++)
	{
		if (this->zAnimalObjects[i]->GetID() == id)
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
		position = this->SearchForPlayerObject(id);
		this->zPlayerObjects.erase(this->zPlayerObjects.begin() + position);
		break;
	case OBJECT_TYPE_ANIMAL:
		position = this->SearchForAnimalObject(id);
		this->zAnimalObjects.erase(this->zAnimalObjects.begin() + position);
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
	for (auto x = this->zPlayerObjects.begin(); x < this->zPlayerObjects.end(); x++)
	{
		(*x)->Update(deltaTime);
	}

	for (auto x = this->zAnimalObjects.begin(); x < this->zAnimalObjects.end(); x++)
	{
		(*x)->Update(deltaTime);
	}

	for (auto x = this->zDynamicObjects.begin(); x < this->zDynamicObjects.end(); x++)
	{
		(*x)->Update(deltaTime);
	}
}

bool WorldObjectManager::AddMapObject( iMesh* object )
{
	if (object)
	{
		this->zMapObjects.push_back(object);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddTerrain(iTerrain* terrain)
{
	if (terrain)
	{
		this->zTerrain = terrain;
		return true;
	}
	return false;
}

iMesh* WorldObjectManager::GetMapObject(unsigned int position)
{
	if (position < this->zMapObjects.size())
	{
		return this->zMapObjects[position];
	}
	return NULL;
}

iTerrain* WorldObjectManager::GetTerrain()
{
	return this->zTerrain;
}

PlayerObject* WorldObjectManager::SearchAndGetPlayerObject(const unsigned int id)
{
	int position = this->SearchForPlayerObject(id);
	return this->zPlayerObjects[position];
}

AnimalObject* WorldObjectManager::SearchAndGetAnimalObject(const unsigned int id)
{
	int position = this->SearchForAnimalObject(id);
	return this->zAnimalObjects[position];
}

StaticObject* WorldObjectManager::SearchAndGetStaticObject(const unsigned int id)
{
	int position = this->SearchForStaticObject(id);
	return this->zStaticObjects[position];
}

DynamicObject* WorldObjectManager::SearchAndGetDynamicObject(const unsigned int id)
{
	int position = this->SearchForDynamicObject(id);
	return this->zDynamicObjects[position];
}