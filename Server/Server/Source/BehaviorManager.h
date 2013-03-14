#pragma once

#include <set>
#include <World/Observer.h>

class Behavior;


class BehaviorManager : public Observed
{
	std::set<Behavior*> zBehaviors;

public:
	virtual ~BehaviorManager();

	// Management
	void AddBehavior(Behavior* behavior);
	void RemoveBehavior(Behavior* behavior);

	// Update Behaviors
	void Update(float dt);
};