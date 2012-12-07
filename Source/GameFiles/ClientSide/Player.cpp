#include "GameFiles/ClientSide/Player.h"
#define PI 3.14159265358979323846f
void Player::Update(float deltaTime)
{
	Vector3 currentPosition = this->zMesh->GetPosition();
	
	//Acceleration
	float t = (-cos(PI * deltaTime) * 0.5f) + 0.5f;//pow(deltaTime, 2);
	LinearInterpolation(currentPosition, this->zEndPosition, t);
	this->SetObjectPosition(currentPosition);
}
void Player::LinearInterpolation(Vector3& CurrPos, const Vector3& newPos, float t)
{
	Vector3 temp = CurrPos + (newPos - CurrPos) * t * zVelocity;
	
	CurrPos = temp;
}
Player::Player() : WorldObject()
{
	this->zPlayerState = STATE_IDLE;
	this->zEndPosition = Vector3(0,0,0);
	this->zVelocity = 1000.0f;
}