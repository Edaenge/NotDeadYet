#include "PlayerGhostBehavior.h"
#include "Player.h"

const int MAX_VELOCITY = 5.0f;
const float ACCELERATION = 5.0f;
const float PLAYERHEIGHT = 0.5f;

PlayerGhostBehavior::PlayerGhostBehavior( Actor* actor, World* world, Player* player ) : PlayerBehavior(actor, world, player)
{

}

PlayerGhostBehavior::~PlayerGhostBehavior()
{

}

bool PlayerGhostBehavior::Update( float dt )
{
	KeyStates keyStates = this->zPlayer->GetKeys();
	Vector3 newPlayerPos;
	Vector3 moveDir = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 currentPlayerPos = this->zActor->GetPosition();

	//Get Directions
	Vector3 currentPlayerDir = this->zActor->GetDir();
	Vector3 currentPlayerUp = this->zActor->GetUpVector();
	currentPlayerDir.Normalize();
	Vector3 currentPlayerRight = currentPlayerUp.GetCrossProduct(currentPlayerDir);
	currentPlayerRight.Normalize();
	Vector3 currentGroundNormal = this->zWorld->CalcNormalAt(currentPlayerPos.GetXZ());

	// Calc the movement vector
	moveDir += currentPlayerDir * (float)(keyStates.GetKeyState(KEY_FORWARD) - //if KEY_BACKWARD then currentPlayerDir inverse 
		keyStates.GetKeyState(KEY_BACKWARD));
	moveDir += currentPlayerRight * (float)(keyStates.GetKeyState(KEY_LEFT) - 
		keyStates.GetKeyState(KEY_RIGHT));
	moveDir *= 10.0f;

	this->zVelocity += moveDir * dt;

	if(this->zVelocity.GetLength() > MAX_VELOCITY)
	{
		this->zVelocity.Normalize();
		this->zVelocity *= MAX_VELOCITY;
	}

	newPlayerPos = currentPlayerPos + this->zVelocity * dt;

	float newGroundHeight = zWorld->CalcHeightAtWorldPos(newPlayerPos.GetXZ()) + PLAYERHEIGHT;
	if(newGroundHeight > newPlayerPos.y)
	{
		newPlayerPos.y = newGroundHeight;
	}
	this->zActor->SetPosition(newPlayerPos);
	this->zAnchor->position = newPlayerPos.GetXZ();
	this->zAnchor->radius = SIGHTRADIUS;

	return true;
}
