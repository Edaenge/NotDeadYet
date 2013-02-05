#pragma once

#include <Observer.h>

class ActorSynchronizer;
class Actor;

class ActorManager : public Observed, public Observer
{
	ActorSynchronizer *zSynch;
public:
	ActorManager( ActorSynchronizer* syncher );

	void AddActor( Actor* actor );
	void RemoveActor( Actor* actor );
};