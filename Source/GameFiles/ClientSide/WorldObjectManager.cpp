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

bool WorldObjectManager::AddObject(PlayerObject* new_PlayerObject)
{
	if (new_PlayerObject)
	{
		this->zPlayerObjects.push_back(new_PlayerObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(AnimalObject* new_AnimalObject)
{
	if (new_AnimalObject)
	{
		this->zAnimalObjects.push_back(new_AnimalObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(StaticObject* new_StaticObject)
{
	if (new_StaticObject)
	{
		this->zStaticObjects.push_back(new_StaticObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject(DynamicObject* new_DynamicObject)
{
	if (new_DynamicObject)
	{
		this->zDynamicObjects.push_back(new_DynamicObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::AddObject( DeadPlayerObject* new_DeadPlayerObject )
{
	if (new_DeadPlayerObject)
	{
		this->zDeadPlayerObjects.push_back(new_DeadPlayerObject);
		return true;
	}
	return false;
}

bool WorldObjectManager::RemoveObject(const unsigned int Type, const int Index)
{
	if (Type == OBJECT_TYPE_PLAYER)
	{
		if (Index < this->zPlayerObjects.size())
		{
			PlayerObject* playerObj = this->zPlayerObjects[Index];
			this->zPlayerObjects.erase(this->zPlayerObjects.begin() + Index);
			if (playerObj)
			{
				delete playerObj;
				playerObj = NULL;
			}
			return true;
		}
	}
	if (Type == OBJECT_TYPE_ANIMAL)
	{
		if (Index < this->zAnimalObjects.size())
		{
			AnimalObject* animalObj = this->zAnimalObjects[Index];
			this->zAnimalObjects.erase(this->zAnimalObjects.begin() + Index);
			if (animalObj)
			{
				delete animalObj;
				animalObj = NULL;
			}
			return true;
		}
	}
	if (Type == OBJECT_TYPE_STATIC_OBJECT)
	{
		if (Index < this->zStaticObjects.size())
		{
			StaticObject* staticObj = this->zStaticObjects[Index];
			this->zStaticObjects.erase(this->zStaticObjects.begin() + Index);
			if (staticObj)
			{
				delete staticObj;
				staticObj = NULL;
			}
			return true;
		}
	}
	if (Type == OBJECT_TYPE_DYNAMIC_OBJECT)
	{
		if (Index < this->zDynamicObjects.size())
		{
			DynamicObject* dynamicObj = this->zDynamicObjects[Index];
			this->zDynamicObjects.erase(this->zDynamicObjects.begin() + Index);
			if (dynamicObj)
			{
				delete dynamicObj;
				dynamicObj = NULL;
			}
			return true;
		}
	}
	if (Type == OBJECT_TYPE_DYNAMIC_OBJECT)
	{
		if (Index < this->zDeadPlayerObjects.size())
		{
			DeadPlayerObject* deadPlayerObj = this->zDeadPlayerObjects[Index];
			this->zDeadPlayerObjects.erase(this->zDeadPlayerObjects.begin() + Index);
			if (deadPlayerObj)
			{
				delete deadPlayerObj;
				deadPlayerObj = NULL;
			}
			return true;
		}
	}
	return false;
}

PlayerObject* WorldObjectManager::GetPlayerObject(const unsigned int Index)
{
	if (Index < this->zPlayerObjects.size())
		return this->zPlayerObjects[Index];

	return NULL;
}

AnimalObject* WorldObjectManager::GetAnimalObject(const unsigned int Index)
{
	if (Index < this->zAnimalObjects.size())
		return this->zAnimalObjects[Index];

	return NULL;
}

StaticObject* WorldObjectManager::GetStaticObject(const unsigned int Index)
{
	if (Index < this->zStaticObjects.size())
		return this->zStaticObjects[Index];
	
	return NULL;
}

DynamicObject* WorldObjectManager::GetDynamicObject(const unsigned int Index)
{
	if (Index < this->zDynamicObjects.size())
		return this->zDynamicObjects[Index];

	return NULL;
}

DeadPlayerObject* WorldObjectManager::GetDeadPlayerObject(const unsigned int Index)
{
	if (Index < this->zDeadPlayerObjects.size())
		return this->zDeadPlayerObjects[Index];

	return NULL;
}

int WorldObjectManager::SearchForObject(const unsigned int type, const int ID)
{
	int index;
	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		index = this->SearchForPlayerObject(ID);
		break;
	case OBJECT_TYPE_ANIMAL:
		index = this->SearchForAnimalObject(ID);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		index = this->SearchForStaticObject(ID);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		index = this->SearchForDynamicObject(ID);
		break;
	case OBJECT_TYPE_DEAD_PLAYER:
		index = this->SearchForDeadPlayerObject(ID);
	default:
		index = -1;
		break;
	}
	return index;
}

int WorldObjectManager::SearchForPlayerObject(const int ID)
{
	for (unsigned int i = 0; i < this->zPlayerObjects.size(); i++)
	{
		if (this->zPlayerObjects[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForStaticObject(const int ID)
{
	for (unsigned int i = 0; i < this->zStaticObjects.size(); i++)
	{
		if (this->zStaticObjects[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForDynamicObject(const int ID)
{
	for (unsigned int i = 0; i < this->zDynamicObjects.size(); i++)
	{
		if (this->zDynamicObjects[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForAnimalObject(const int ID)
{
	for (unsigned int i = 0; i < this->zAnimalObjects.size(); i++)
	{
		if (this->zAnimalObjects[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

int WorldObjectManager::SearchForDeadPlayerObject(const int ID)
{
	for (unsigned int i = 0; i < this->zDeadPlayerObjects.size(); i++)
	{
		if (this->zDeadPlayerObjects[i]->GetID() == ID)
		{
			return i;
		}
	}
	return -1;
}

bool WorldObjectManager::SearchAndRemove(const unsigned int type, const int ID)
{
	int index;

	switch(type)
	{
	case OBJECT_TYPE_PLAYER:
		index = this->SearchForPlayerObject(ID);
		RemoveObject(OBJECT_TYPE_PLAYER, ID);
		break;
	case OBJECT_TYPE_ANIMAL:
		index = this->SearchForAnimalObject(ID);
		RemoveObject(OBJECT_TYPE_ANIMAL, ID);
		break;
	case OBJECT_TYPE_STATIC_OBJECT:
		index = this->SearchForStaticObject(ID);
		RemoveObject(OBJECT_TYPE_STATIC_OBJECT, ID);
		break;
	case OBJECT_TYPE_DYNAMIC_OBJECT:
		index = this->SearchForDynamicObject(ID);
		RemoveObject(OBJECT_TYPE_DYNAMIC_OBJECT, ID);
		break;
	case OBJECT_TYPE_DEAD_PLAYER:
		index = this->SearchForDynamicObject(ID);
		RemoveObject(OBJECT_TYPE_DEAD_PLAYER, ID);
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

PlayerObject* WorldObjectManager::SearchAndGetPlayerObject(const int ID)
{
	int position = this->SearchForPlayerObject(ID);
	return this->zPlayerObjects[position];
}

AnimalObject* WorldObjectManager::SearchAndGetAnimalObject(const int ID)
{
	int position = this->SearchForAnimalObject(ID);
	return this->zAnimalObjects[position];
}

StaticObject* WorldObjectManager::SearchAndGetStaticObject(const int ID)
{
	int position = this->SearchForStaticObject(ID);
	return this->zStaticObjects[position];
}

DynamicObject* WorldObjectManager::SearchAndGetDynamicObject(const int ID)
{
	int position = this->SearchForDynamicObject(ID);
	return this->zDynamicObjects[position];
}

DeadPlayerObject* WorldObjectManager::SearchAndGetDeadPlayerObject(const int ID)
{
	int position = this->SearchForDeadPlayerObject(ID);
	return this->zDeadPlayerObjects[position];
}