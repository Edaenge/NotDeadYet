#include "GameFiles/ClientSide/DynamicObject.h"

void DynamicObject::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();

	float t = this->GetInterpolationType(deltaTime, IT_SMOOTH_STEP);

	this->LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetPosition(currentPosition);
}

DynamicObject::DynamicObject(const unsigned int id) : MovingObject(id)
{
	this->zType = 0;
	this->zWeight = 0;
	this->zStackSize = 1;
	this->zName = "Unknown";
	this->zIconPath = "Unknown";
	this->zDescription = "Unknown";
}
