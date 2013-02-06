#pragma once

#include <Observer.h>
#include "ClientData.h"

class PlayerConnectedEvent : public Event
{
	virtual ~PlayerConnectedEvent();
	ClientData* clientData;
};

class PlayerDisconnectedEvent : public Event
{
	virtual ~PlayerDisconnectedEvent();
	ClientData* clientData;};

class PlayerDiedEvent : public Event
{
public:
	virtual ~PlayerDiedEvent();
	ClientData* clientData;
};

class PlayerUpdateEvent : public Event
{
public:
	virtual ~PlayerUpdateEvent();
	ClientData* clientData;
};








class ClientDataEvent : public Event
{
	virtual ~ClientDataEvent();
	ClientData* clientData;
	
	Vector3 position;
	Vector3 upVector;
	Vector3 direction;
	Vector4 rotation;
	float frameTime;
};