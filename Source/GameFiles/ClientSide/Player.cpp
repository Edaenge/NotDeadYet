#include "GameFiles/ClientSide/Player.h"

#define PI 3.14159265358979323846f

Player::Player() : WorldObject()
{
	this->zPlayerState = STATE_IDLE;
	this->zEndPosition = Vector3(0,0,0);
	this->zVelocity = 1000.0f;
}
void Player::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();
	
	float t;
	//Cosine
	//t = (-cos(PI * deltaTime) * 0.5f) + 0.5f;

	//Acceleration
	t = pow(deltaTime, 2);

	//Smooth Step
	//t = pow(deltaTime, 2) * (3 - 2 * deltaTime);

	this->LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetObjectPosition(currentPosition);
}
void Player::LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float t)
{
	Vector3 temp = CurrPos + (newPos - CurrPos) * t * zVelocity;
	
	CurrPos = temp;
}