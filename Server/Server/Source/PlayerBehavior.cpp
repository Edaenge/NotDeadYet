#include "PlayerBehavior.h"
#include "Actor.h"
#include "Player.h"


PlayerBehavior::PlayerBehavior( Actor* actor, World* world, Player* player ) : 
	Behavior(actor, world),
	zVelocity(0.0f, 0.0f, 0.0f),
	zPlayer(player){
}

bool PlayerBehavior::Update( float dt )
{
	/*Vector3 newPlayerPos;
	Vector3 moveDir = Vector3(0,0,0);

	Vector3 currentPlayerPos = this->zActor->GetPosition();

	//Get Directions
	Vector3 currentPlayerDir = this->zActor->GetDir();
	Vector3 currentPlayerUp = this->zActor->GetUpVector();
	currentPlayerUp.y = 0; // Need a vector that is horizontal to X and Z
	currentPlayerUp.Normalize();
	Vector3 currentPlayerRight = currentPlayerUp.GetCrossProduct(currentPlayerDir);
	Vector3 currentGroundNormal = this->zWorld->CalcNormalAt(currentPlayerPos.GetXZ());
	float currentGroundHeights = this->zWorld->CalcHeightAtWorldPos(currentPlayerPos.GetXZ());

	// Calc the movement vector
	moveDir += currentPlayerDir * (float)(this->zKeyState->GetKeyState(KEY_FORWARD) - //if KEY_BACKWARD then currentPlayerDir inverse 
		this->zKeyState->GetKeyState(KEY_BACKWARD));
	moveDir += currentPlayerRight * (float)(this->zKeyState->GetKeyState(KEY_LEFT) - 
		this->zKeyState->GetKeyState(KEY_RIGHT));
	moveDir.Normalize();
	moveDir *= 10.0f;

	this->zVelocity += (moveDir + GRAVITY) * dt;

	if(this->zVelocity.GetLength() > MAX_VELOCITY)
	{
		this->zVelocity.Normalize();
		this->zVelocity *= MAX_VELOCITY;
	}

	newPlayerPos = currentPlayerPos + this->zVelocity * dt;
	
	float newGroundHeight = zWorld->CalcHeightAtWorldPos(newPlayerPos.GetXZ()) + 1.7f;
	if(newGroundHeight > newPlayerPos.y)
	{
		Vector3 newGroundNormal = zWorld->CalcNormalAt(newPlayerPos.GetXZ());
		this->zVelocity = newGroundNormal * this->zVelocity.GetLength() * ELASTICITY;
	}
	this->zActor->SetPosition(newPlayerPos);*/
	return true;
}

bool PlayerBehavior::ProcessClientData( Vector3 direction, Vector4 rotation )
{
	return true;
}
