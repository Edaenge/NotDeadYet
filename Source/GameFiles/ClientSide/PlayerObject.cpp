#include "GameFiles/ClientSide/PlayerObject.h"

#define PI 3.14159265358979323846f

void PlayerObject::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();

	float t;
	//Cosine
	//t = (-cos(PI * deltaTime) * 0.5f) + 0.5f;

	//Acceleration
	//t = pow(deltaTime, 2);

	//Smooth Step
	t = pow(deltaTime, 2) * (3 - 2 * deltaTime);

	this->LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetPosition(currentPosition);
}