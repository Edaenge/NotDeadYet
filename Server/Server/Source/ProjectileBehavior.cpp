#include "ProjectileBehavior.h"


ProjectileBehavior::ProjectileBehavior( Actor* actor, World* world ) : Behavior(actor, world)
{

}

bool ProjectileBehavior::Update( float dt )
{

	return true;
}