#include "AudioManager.h"
#include "EventHandle.h"
#include "Safe.h"

AudioManager* AudioManager::pnk = NULL;
bool AudioManager::isInited = false;


AudioManager::AudioManager()
{

	currentCue = NULL;

	this->groups = NULL;
	this->cats = NULL;
	this->system = NULL;
	this->mSystem = NULL;
	this->currentCue = NULL;
	this->projectFile = NULL;
	this->root = NULL;
	this->eventGroups = NULL;
	this->categories = NULL;

}


AudioManager* AudioManager::GetInstance()
{

	if(!isInited)
	{

		pnk = new AudioManager();
		pnk->init();
		isInited = true;

	}

	return pnk;

}


void AudioManager::ReleaseInstance()
{

	if(isInited)
	{

		pnk->release();
		delete pnk;
		isInited = false;

	}
}


AudioManager::~AudioManager()
{

	if(eventGroups)
		delete[] eventGroups;
	eventGroups = NULL;

	if(categories)
		delete[] categories;
	categories = NULL;

}


int AudioManager::init()
{
	
	FMOD_RESULT result;
	result = FMOD::EventSystem_Create(&system);		// Create the main system object.

	if (result != FMOD_OK)
	{

		return result;

	}

	result = system->init(64, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL);	// Initialize FMOD.
	
	if (result != FMOD_OK)
	{

		return result;

	}
	result = system->getMusicSystem(&mSystem);
	return result;

}


int AudioManager::PlayMusicCue(int cue)
{
	FMOD_RESULT result;

	if(currentCue != NULL)
	{
		result = currentCue->end();	
		
		if (result != FMOD_OK)
		{
			return result;
		}
	}
	
	result = mSystem->prepareCue(1,&currentCue);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	result = currentCue->begin();
	return result;

}


int AudioManager::TogglePauseMusicCue()
{

	bool isPaused = false;
	FMOD_RESULT result = mSystem->getPaused(&isPaused);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	result = mSystem->setPaused(!isPaused);
	return result;

}


int AudioManager::StopMusicCue()
{

	return currentCue->end();

}


int AudioManager::SetMusicParameters(int parameter,float value)
{

	return mSystem->setParameterValue(parameter,value);

}


void AudioManager::Update()
{

	system->update();

}


int AudioManager::LoadFEV(std::string file)
{
	
	FMOD_RESULT result= system->load(file.c_str(),0,&projectFile);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	result = system->getNumCategories(&cats);

	categories = new FMOD::EventCategory*[cats+1];
	
	if (result != FMOD_OK)
	{

		return result;

	}

	for(int i=0;i<cats;i++)
	{

		FMOD::EventCategory* temp;
		result = system->getCategoryByIndex(i,&temp);
		
		if (result != FMOD_OK)
		{

			return result;

		}

		categories[i] = temp;

	}

	result = projectFile->getGroupByIndex(0,false,&root);

	if (result != FMOD_OK)
	{

		return result;

	}

	result = root->getNumGroups(&groups);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	eventGroups = new FMOD::EventGroup*[groups];
	return result;

}


int AudioManager::SetVolume(int index, float vol)
{
	
	return categories[index]->setVolume(vol);

}


int AudioManager::UnloadFEV()
{

	return projectFile->release();

}


int AudioManager::release()
{
	
	FMOD_RESULT result = system->release();
	return result;

}


int AudioManager::LoadEventData(int group)
{

	FMOD::EventGroup* eGroup;
	FMOD_RESULT result = root->getGroupByIndex(group,false,&eGroup);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	//Load the Data
	result = eGroup->loadEventData(FMOD_EVENT_RESOURCE_STREAMS_AND_SAMPLES,FMOD_EVENT_DEFAULT);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	eventGroups[group] = eGroup;

	//result = insertEvents(eGroup,result,group);
	return result;

}


int AudioManager::UnloadEventData(int group)
{

	FMOD_RESULT result;
	//Free data
	result = eventGroups[group]->freeEventData(0,true);
	return result;

}


int AudioManager::GetEventHandle(int group,int eventId,IEventHandle* &handle)
{

	FMOD::Event* temp;
	FMOD_RESULT result = eventGroups[group]->getEventByIndex(eventId,FMOD_EVENT_DEFAULT,&temp);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	handle = new EventHandle(temp);
	return result;

}


int AudioManager::GetEventHandle(int eventId,IEventHandle* &handle)
{

	FMOD::Event* temp;
	FMOD_RESULT result = projectFile->getEventByProjectID(eventId,FMOD_EVENT_DEFAULT,&temp);
	
	if (result != FMOD_OK)
	{

		return result;

	}

	handle = new EventHandle(temp);
	return result;

}


int AudioManager::SetPlayerPosition(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up)
{
	return system->set3DListenerAttributes(0, &position, 0, &forward, &up);
}