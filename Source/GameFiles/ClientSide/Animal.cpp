#include "GameFiles/ClientSide/Animal.h"

#define PI 3.14159265358979323846f

Animal::Animal() : WorldObject()
{
	this->zState = STATE_IDLE;
	this->zEndPosition = Vector3(0,0,0);
	this->zVelocity = 1000.0f;
}

void Animal::Update(float deltaTime)
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

void Animal::LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float t)
{
	Vector3 temp = CurrPos + (newPos - CurrPos) * t * zVelocity;

	CurrPos = temp;
}