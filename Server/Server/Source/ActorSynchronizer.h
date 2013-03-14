#pragma once
/************************************************************************/
/* Class that gather all updates that is made on actors.				*/
/* After that you can send update info to clients.						*/
/************************************************************************/

#include <World/Observer.h>
#include "ClientData.h"
#include "NetworkMessageConverter.h"

#include <set>
#include <map>

class Actor;
class BioActor;
class ServerFramePacket;
class NewActorPacket;
class PhysicalConditionPacket;


class ActorSynchronizer : public Observer
{
private:
	ServerFramePacket* zFrameData;
	NewActorPacket* zActorData;

	std::set<Actor*> zUpdateSet;
	std::set<Actor*> zNewActorSet;
	std::set<unsigned int> zRemoveActorSet;

	std::map<ClientData*, PhysicalConditionPacket*> zIndividualPhysicalConditions;

	void RegisterActor(ClientData* cd);
	void RemoveActor(ClientData* cd);
	void AddAnimation(BioActor* bActor);
public:
	ActorSynchronizer();
	virtual ~ActorSynchronizer();

	void SendUpdatesTo(ClientData* cd);

	void ClearAll();

protected:
	virtual void OnEvent(Event* e);

};