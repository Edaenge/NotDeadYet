#include "PhysicsResourceManager.h"
 

PhysicsResourceManager::PhysicsResourceManager()
{
	this->mutex = CreateMutex(NULL, false, NULL);
}

PhysicsResourceManager::~PhysicsResourceManager()
{
	//Do not release device & device context, this is done by DxManager.

	//Object data resources
	map<std::string, ObjectDataResource*>::iterator objDataIterator;
	for(objDataIterator = this->zObjectDataResources.begin(); objDataIterator != this->zObjectDataResources.end(); objDataIterator++)
	{
		if(objDataIterator->second)
		{
			//An object data cannot be deleted since it's destructor is private to force correct use of texture creation/deletion.
			//Instead decrease reference count until it deletes itself.
			int refCount = objDataIterator->second->GetReferenceCount();
			MaloW::Debug("WARNING: Resource manager deleted the object resource: '" 
				+ objDataIterator->second->GetName() 
				+ "'; missing decrease(s) in reference counter somewhere. Occurrences: " 
				+ MaloW::convertNrToString((float)(refCount - 1))
				+ ". Keep in mind that the cause can be PreLoadResources()-function if the resource was loaded but not used.");
			for(int i = 0; i < refCount; i++)
			{
				objDataIterator->second->DecreaseReferenceCount();
			}

			objDataIterator->second = NULL;
		}
	}
	this->zObjectDataResources.clear();
	


	CloseHandle(this->mutex);
}




void PhysicsResourceManager::PreLoadResources(unsigned int nrOfResources, char const* const* const resourcesFileNames)
{
	if(resourcesFileNames != NULL)
	{
		for(unsigned int i = 0; i < nrOfResources; i++)
		{
			const char* resourcesFileName = resourcesFileNames[i];
			if(resourcesFileName != NULL)
			{
				string tmpFileName = string(resourcesFileName); 
				if(tmpFileName.substr(tmpFileName.length() - 4) == ".obj") 
				{
					//First check if the resource has already been loaded.
					auto objData = this->zObjectDataResources.find(resourcesFileName);
					if(objData == this->zObjectDataResources.end())
					{
						ObjectDataResource* objDataResPtr = this->LoadObjectDataResourceFromFile(resourcesFileName);
						if(objDataResPtr != NULL)
						{
							//Decrease reference count since we're Preloading and no object yet has a reference to the resource.
							this->zObjectDataResources[resourcesFileName]->DecreaseReferenceCount();
						}
						//else write debug info, this is done by LoadObjectDataResourceFromFile(..).
					}
					else
					{
						MaloW::Debug("INFO: ResourceManager: PreLoadResources(): the resource: '" + string(resourcesFileName) + "' has already been loaded.");
					}
				}
			}
		}
	}
}



ObjectDataResource* PhysicsResourceManager::LoadObjectDataResourceFromFile(const char* filePath)
{
	if(this->mutex)
	{
		WaitForSingleObject(this->mutex, INFINITE);
	}
	else 
		MaloW::Debug("Mutex is broken / hasn't been created / has been closed for Resourcemanager Loadobjectdata.");



	auto objData = this->zObjectDataResources.find(filePath);
	//If the object data resource was not found in the array, create it.
	if(objData == this->zObjectDataResources.end())
	{
		//Create and load object data.
		ObjData* objectData = NULL;
		ObjLoader oj;
		objectData = oj.LoadObjFile(string(filePath));

		if(objectData == NULL)
		{
			string dbgStr = "WARNING: Failed to load object data from file: ";
			dbgStr += filePath;
			MaloW::Debug(dbgStr);
			
			//Release mutex and return.
			ReleaseMutex(this->mutex);
			return NULL;
		}
		else
		{
			//Create if loading was successful.
			this->zObjectDataResources[filePath] = new ObjectDataResource(filePath, objectData);
			//Increase reference count.
			this->zObjectDataResources[filePath]->IncreaseReferenceCount();
			
			//Release mutex and return resource.
			ReleaseMutex(this->mutex);
			//Return newly created object data resource.
			return this->zObjectDataResources[filePath];
		}
	}

	//If the object data resource already exists, increase reference counter & return it.
	this->zObjectDataResources[filePath]->IncreaseReferenceCount();
	
	//Release mutex and return.
	ReleaseMutex(this->mutex);
	return objData->second;
}

void PhysicsResourceManager::UnloadObjectDataResource(const char* filePath)
{
	if(this->mutex)
	{
		WaitForSingleObject(this->mutex, INFINITE);
	}
	else 
		MaloW::Debug("Mutex is broken / hasn't been created / has been closed for Resourcemanager UnloadObjectData.");


	auto objData = this->zObjectDataResources.find(filePath);
	//If the object data resource was found in the array, decrease its reference counter.
	if(objData != this->zObjectDataResources.end())
	{
		ObjectDataResource* objectData = objData->second;
		objectData->DecreaseReferenceCount();

		//If reference count is 1, no objects other than the resource manager itself has a reference to it.
		if(objectData->GetReferenceCount() == 1)
		{
			//therefore delete it by decreasing its reference count one more time.
			objectData->DecreaseReferenceCount();
			//Remove object data resource from table.
			this->zObjectDataResources.erase(objData);
		}
	}
	else
	{
		MaloW::Debug("WARNING: ResourceManager::UnloadObjectDataResource(): Could not find the following object data resource to unload: '" + string(filePath) + "'.");
	}

	//Release mutex.
	ReleaseMutex(this->mutex);
}









namespace
{
	static PhysicsResourceManager* physicsResourceManager = NULL;
}


PhysicsResourceManager* GetPhysicsResourceManager()
{
	if(!physicsResourceManager)
	{
		physicsResourceManager = new PhysicsResourceManager();
	}

	return physicsResourceManager;
}

bool FreePhysicsResourceManager()
{
	if(physicsResourceManager)
	{
		delete physicsResourceManager;
		physicsResourceManager = NULL;
	}
	else
	{
		return false;
	}	

	return true;
}