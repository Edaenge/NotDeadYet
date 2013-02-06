#pragma once
/************************************************************************/
/* Class that gather all updates that is made on actors.				*/
/* After that you can send update info to clients.						*/
/************************************************************************/

#include "BioActor.h"
#include "Observer.h"
#include "ClientData.h"
#include "NetworkMessageConverter.h"

#include <set>

class ActorSynchronizer : public Observer
{
private:
	std::set<Actor*> zUpdateSet;
public:
	ActorSynchronizer();
	virtual ~ActorSynchronizer();

	bool SendUpdatesTo(ClientData* cd);

	bool ClearAll();

protected:
	virtual void OnEvent(Event* e);
};