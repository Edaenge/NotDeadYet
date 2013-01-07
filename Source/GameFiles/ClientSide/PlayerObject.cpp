#include "GameFiles/ClientSide/PlayerObject.h"

void PlayerObject::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();

	float t = this->GetInterpolationType(deltaTime, IT_SMOOTH_STEP);
	
	this->LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetPosition(currentPosition);
}