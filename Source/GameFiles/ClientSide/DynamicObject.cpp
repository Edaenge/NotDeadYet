#include "GameFiles/ClientSide/DynamicObject.h"

#define PI 3.14159265358979323846f

DynamicObject::DynamicObject(const unsigned int id) : MovingObject(id)
{
	this->zState = STATE_IDLE;
	this->zEndPosition = Vector3(0,0,0);
	this->zVelocity = 1000.0f;
}

void DynamicObject::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();

	//Cosine
	//float t = (-cos(PI * deltaTime) * 0.5f) + 0.5f;

	//Acceleration
	//float t = pow(deltaTime, 2);

	//Smooth Step
	float t = pow(deltaTime, 2) * (3 - 2 * deltaTime);

	this->LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetPosition(currentPosition);
}