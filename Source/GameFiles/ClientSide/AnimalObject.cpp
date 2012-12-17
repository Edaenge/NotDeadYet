#include "GameFiles/ClientSide/AnimalObject.h"

#define PI 3.14159265358979323846f

void AnimalObject::Update(float deltaTime)
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