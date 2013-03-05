#pragma once

#include <Observer.h>
#include <vector.h>
#include "ClientData.h"
#include "Actor.h"

class PlayerExhausted : public Event
{
public:
	virtual ~PlayerExhausted() {}

	Vector3 zPos;
};


class PlayerActorPhysicalConditionHungerEvent : public Event
{
public:
	virtual ~PlayerActorPhysicalConditionHungerEvent() {}

	Actor* zPlayerActor;
};

class PlayerActorPhysicalConditionHydrationEvent : public Event
{
public:
	virtual ~PlayerActorPhysicalConditionHydrationEvent() {}

	Actor* zPlayerActor;
};