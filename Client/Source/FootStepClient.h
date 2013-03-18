#pragma once

#include <World/World.h>

class ClientActorManager;


class FootStepClient : public Observer
{
	ClientActorManager* zActorManager;

public:
	FootStepClient(ClientActorManager* actorManager);
	virtual ~FootStepClient();
};