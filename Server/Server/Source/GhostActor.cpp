#include "GhostActor.h"


GhostActor::GhostActor(const bool genID /*= false*/, const Vector3& position)
{
	if (genID)
		this->zID = this->GenerateID();

	this->zPosition = position;
	this->zDirection = Vector3(0,0,1);

	this->zVelocity = 5.0f;
	this->zEnergy = 100.0f;
}

GhostActor::GhostActor(Player* player)
{
	this->zPlayer = player;
}

GhostActor::~GhostActor()
{

}

void GhostActor::Update( float deltaTime )
{
	Vector3 movement = Vector3(0,0,0);
	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
	{
		movement += this->zDirection;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		movement -= this->zDirection;
	}
	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDirection);
		movement += right;
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDirection);
		movement -= right;
	}

	movement.Normalize();
	movement *= deltaTime * this->zVelocity;

	this->zPosition += movement;
}

std::string GhostActor::ToMessageString(NetworkMessageConverter* NMC)
{
	std::string msg = NMC->Convert(MESSAGE_TYPE_POSITION, this->zPosition);

	return msg;
}