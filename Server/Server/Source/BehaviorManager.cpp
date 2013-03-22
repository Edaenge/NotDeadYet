#include "BehaviorManager.h"
#include "Behavior.h"


BehaviorManager::BehaviorManager(Observer* observer) : 
	Observed(observer)
{

}

BehaviorManager::~BehaviorManager()
{
	for( auto i = zBehaviors.cbegin(); i != zBehaviors.cend(); ++i )
	{
		delete *i;
	}
}

void BehaviorManager::AddBehavior(Behavior* behavior)
{
	if ( behavior )
	{
		if ( zBehaviorsInUse )
		{
			zBehaviorsUpdates[behavior]--;
		}
		else
		{
			// Insert To Set
			zBehaviors.insert(behavior);

			// Notify Observers
			BehaviorAddedEvent BAE;
			BAE.zBehavior = behavior;
			NotifyObservers(&BAE);
		}
	}
}

void BehaviorManager::RemoveBehavior(Behavior* behavior)
{
	if ( behavior )
	{
		if ( zBehaviorsInUse )
		{
			zBehaviorsUpdates[behavior]--;
		}
		else
		{
			// Erase From Set
			zBehaviors.erase(behavior);

			// Notify Observers
			BehaviorRemovedEvent BRE;
			BRE.zBehavior = behavior;
			NotifyObservers(&BRE);
		}
	}
}

void BehaviorManager::Update(float dt)
{
	zBehaviorsInUse = true;

	// Update behaviors
	for( auto i = zBehaviors.cbegin(); i != zBehaviors.cend(); ++i )
	{
		auto deleteCheck = zBehaviorsUpdates.find(*i);
		if ( deleteCheck == zBehaviorsUpdates.cend() || deleteCheck->second > 0 )
		{
			if ( (*i)->IsAwake() )
			{
				// One behavior won't cause whole server to not work
				try
				{
					if ( (*i)->Update(dt) )
					{
						zBehaviorsUpdates[*i]--;
					}
				}
				catch(...)
				{

				}
			}
		}
	}

	// Update behavior set
	for( auto i = zBehaviorsUpdates.cbegin(); i != zBehaviorsUpdates.cend(); ++i )
	{
		if ( i->second > 0 )
		{
			zBehaviors.insert(i->first);

			// Notify Observers
			BehaviorAddedEvent BAE;
			BAE.zBehavior = i->first;
			NotifyObservers(&BAE);
		}
		else if ( i->second < 0 )
		{
			zBehaviors.erase(i->first);

			// Notify Observers
			BehaviorRemovedEvent BRE;
			BRE.zBehavior = i->first;
			NotifyObservers(&BRE);

			delete i->first;
		}
	}
	zBehaviorsUpdates.clear();

	zBehaviorsInUse = false;
}