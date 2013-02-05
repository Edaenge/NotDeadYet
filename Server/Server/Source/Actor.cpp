#include "Actor.h"

long Actor::zNextAID = 500;

void Actor::SetPosition( const Vector3& pos )
{
	zPos = pos;
	ActorPositionEvent APE;
	APE.zActor = this;
	NotifyObservers(&APE);
}

void Actor::SetRotation( const Vector3& rot )
{
	zRot = rot;
	ActorRotationEvent ARE;
	ARE.zActor = this;
	NotifyObservers(&ARE);
}

void Actor::SetScale( const Vector3& scale )
{
	zScale = scale;
	ActorScaleEvent ASE;
	ASE.zActor = this;
	NotifyObservers(&ASE);
}