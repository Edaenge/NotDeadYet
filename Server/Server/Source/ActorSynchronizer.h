#pragma once
/************************************************************************/
/* Class that gather all updates that is made on actors.				*/
/* After that you can send update info to clients.						*/
/************************************************************************/

#include "Observer.h"
#include "ClientData.h"
#include "NetworkMessageConverter.h"

#include <set>

class Actor;

/*Events when a new actor has been added in ActorManager*/
class ActorAdded : public Event
{
public:
	virtual ~ActorAdded() {}

	Actor* zActor;
};

/*Events when a new actor has been removed from ActorManager*/
class ActorRemoved : public Event
{
public:
	virtual ~ActorRemoved() {}

	Actor* zActor;
};

class ActorSynchronizer : public Observer
{
private:
	std::set<Actor*> zUpdateSet;
	std::set<Actor*> zNewActorSet;
	std::set<Actor*> zRemoveActorSet;

private:
	void RegisterActor(ClientData* cd);
	void RemoveActor(ClientData* cd);

public:
	ActorSynchronizer();
	virtual ~ActorSynchronizer();

	void SendUpdatesTo(ClientData* cd);

	void ClearAll();

protected:
	virtual void OnEvent(Event* e);
};