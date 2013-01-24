#include "WorldObjectManager.h"


WorldObjectManager::WorldObjectManager()
{

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

bool WorldObjectManager::RemoveObject(const unsigned int type, const unsigned int index)
{
	if (type == OBJECT_TYPE_PLAYER)
	{
		if (index < this->zPlayerObjects.size())
		{
			PlayerObject* playerObj = this->zPlayerObjects[index];
			this->zPlayerObjects.erase(this->zPlayerObjects.begin() + index);
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
		if (index < this->zAnimalObjects.size())
		{
			AnimalObject* animalObj = this->zAnimalObjects[index];
			this->zAnimalObjects.erase(this->zAnimalObjects.begin() + index);
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
		if (index < this->zStaticObjects.size())
		{
			StaticObject* staticObj = this->zStaticObjects[index];
			this->zStaticObjects.erase(this->zStaticObjects.begin() + index);
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
		if (index < this->zDynamicObjects.size())
		{
			DynamicObject* dynamicObj = this->zDynamicObjects[index];
			this->zDynamicObjects.erase(this->zDynamicObjects.begin() + index);
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

PlayerObject* WorldObjectManager::GetPlayerObject(const unsigned int index)
{
	if (index < this->zPlayerObjects.size())
		return this->zPlayerObjects[index];

	return NULL;
}

AnimalObject* WorldObjectManager::GetAnimalObject(const unsigned int index)
{
	if (index < this->zAnimalObjects.size())
		return this->zAnimalObjects[index];

	return NULL;
}

StaticObject* WorldObjectManager::GetStaticObject(const unsigned int index)
{
	if (index < this->zStaticObjects.size())
		return this->zStaticObjects[index];
	
	return NULL;
}

DynamicObject* WorldObjectManager::GetDynamicObject(const unsigned int index)
{
	if (index < this->zDynamicObjects.size())
		return this->zDynamicObjects[index];

	return NULL;
}

int WorldObjectManager::SearchForObject(const unsigned int type, const unsigned int id)
{
	int index;
	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		index = this->SearchForPlayerObject(id);
		break;
	case OBJECT_TYPE_ANIMAL:
		index = this->SearchForAnimalObject(id);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		index = this->SearchForStaticObject(id);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		index = this->SearchForDynamicObject(id);
		break;
	default:
		index = -1;
		break;
	}
	return index;
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
	for (unsigned int i = 0; i < this->zDynamicObjects.size(); i++)
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
	int index;

	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		index = this->SearchForPlayerObject(id);
		this->zPlayerObjects.erase(this->zPlayerObjects.begin() + index);
		break;
	case OBJECT_TYPE_ANIMAL:
		index = this->SearchForAnimalObject(id);
		this->zAnimalObjects.erase(this->zAnimalObjects.begin() + index);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		index = this->SearchForStaticObject(id);
		this->zStaticObjects.erase(this->zStaticObjects.begin() + index);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		index = this->SearchForDynamicObject(id);
		this->zDynamicObjects.erase(this->zDynamicObjects.begin() + index);
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