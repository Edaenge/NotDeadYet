#pragma once

#include <set>
#include <map>
#include <World/Observer.h>

class Behavior;

// A behavior has been added to the manager
class BehaviorAddedEvent : public Event
{
public:
	Behavior* zBehavior;
};

// A behavior has been removed from the manager
class BehaviorRemovedEvent : public Event
{
public:
	Behavior* zBehavior;
};

// Behavior manager class
class BehaviorManager : public Observed
{
	// Current set of behaviors
	std::set<Behavior*> zBehaviors;
	
	// Behavior set updates
	std::map<Behavior*, int> zBehaviorsUpdates;
	bool zBehaviorsInUse;

public:
	BehaviorManager(Observer* observer);

	// Destructor deletes remaining behaviors
	virtual ~BehaviorManager();

	// Management
	void AddBehavior(Behavior* behavior);
	void RemoveBehavior(Behavior* behavior);

	// Update behaviors
	void Update(float dt);
};