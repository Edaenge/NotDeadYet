#include "AIBehavior.h"

AIBehavior::AIBehavior(Actor* actor, World* world) : Behavior(actor, world)
{
	this->zTargetSet.clear();
}

AIBehavior::~AIBehavior()
{

}

