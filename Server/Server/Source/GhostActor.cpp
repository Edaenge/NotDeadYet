#include "GhostActor.h"

GhostActor::GhostActor(Player* player) : 
	zPlayer(player)
{
}

GhostActor::~GhostActor()
{

}

/*
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
*/
