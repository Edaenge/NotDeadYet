#include "AIDeerBehavior.h"


AIDeerBehavior::AIDeerBehavior( Actor* actor, World* world ) : Behavior(actor, world)
{

}

bool AIDeerBehavior::Update( float dt )
{
	return true;
}