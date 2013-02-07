#include <World/World.h>
#include <Vector.h>
#include "Player.h"
#include "Actor.h"
#include "PlayerWolfBehavior.h"

const Vector3 GRAVITY = Vector3(0, -9.82f, 0);
const float ELASTICITY = 0.5f;
const float ACCELERATION = 1.0f;
const float PLAYERHEIGHT = 1.0f;

PlayerWolfBehavior::PlayerWolfBehavior( Actor* actor, World* world, Player* player) : 
	PlayerBehavior(actor, world, player)
{

}

PlayerWolfBehavior::~PlayerWolfBehavior()
{

}

bool PlayerWolfBehavior::Update( float dt )
{
	KeyStates keyStates = this->zPlayer->GetKeys();
	Vector3 newPlayerPos;
	Vector3 moveDir = Vector3(0,0,0);

	Vector3 currentPlayerPos = this->zActor->GetPosition();

	//Get Directions
	Vector3 currentPlayerDir = this->zActor->GetDir();
	Vector3 currentPlayerUp = Vector3(0.0f, 1.0f, 0.0f);
	currentPlayerDir.y = 0; // Need a vector that is horizontal to X and Z
	currentPlayerDir.Normalize();
	Vector3 currentPlayerRight = currentPlayerUp.GetCrossProduct(currentPlayerDir);
	currentPlayerRight.Normalize();
	Vector3 currentGroundNormal = this->zWorld->CalcNormalAt(currentPlayerPos.GetXZ());

	// Calc the movement vector
	moveDir += currentPlayerDir * (float)(keyStates.GetKeyState(KEY_FORWARD) - //if KEY_BACKWARD then currentPlayerDir inverse 
		keyStates.GetKeyState(KEY_BACKWARD));

	moveDir += currentPlayerRight * (float)(keyStates.GetKeyState(KEY_LEFT) - 
		keyStates.GetKeyState(KEY_RIGHT));

	moveDir.Normalize();
	moveDir *= 10.0f;

	this->zVelocity += (moveDir + GRAVITY) * dt;

	if(this->zVelocity.GetLength() > MAX_VELOCITY)
	{
		this->zVelocity.Normalize();
		this->zVelocity *= MAX_VELOCITY;
	}

	newPlayerPos = currentPlayerPos + this->zVelocity * dt;

	float newGroundHeight = zWorld->CalcHeightAtWorldPos(newPlayerPos.GetXZ()) + PLAYERHEIGHT;
	if(newGroundHeight > newPlayerPos.y)
	{
		Vector3 newGroundNormal = zWorld->CalcNormalAt(newPlayerPos.GetXZ());
		this->zVelocity = newGroundNormal * this->zVelocity.GetLength() * ELASTICITY;
	}
	this->zActor->SetPosition(newPlayerPos);
	this->zAnchor->position = newPlayerPos.GetXZ();
	this->zAnchor->radius = SIGHTRADIUS;

	return true;
}
