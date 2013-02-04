#include "DeadPlayerObjectActor.h"

DeadPlayerObjectActor::DeadPlayerObjectActor(const bool genID /*= false*/) :  DeadActor(genID)
{
	this->zActorType = ACTOR_TYPE_DEAD_PLAYER;
}

