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

class ActorSynchronizer : public Observer
{
private:
	std::set<Actor*> zUpdateSet;
public:
	ActorSynchronizer();
	virtual ~ActorSynchronizer();

	void SendUpdatesTo(ClientData* cd);

	void ClearAll();

protected:
	virtual void OnEvent(Event* e);
};