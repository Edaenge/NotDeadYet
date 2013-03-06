#include "WorldActor.h"

WorldActor::WorldActor( PhysicsObject* pObj, const float& blockRadius): Actor()
{
	this->SetPhysicsObject(pObj);
	this->zCollisionRadius = blockRadius;
	CalculateCollisionPoints();
}

WorldActor::~WorldActor()
{

}
