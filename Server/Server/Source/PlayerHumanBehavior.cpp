#include <World/World.h>
#include "PlayerHumanBehavior.h"
#include "Player.h"
#include "Actor.h"

const int MAX_VELOCITY = 30;
const Vector3 GRAVITY = Vector3(0, -9.82f, 0);
const float ELASTICITY = 0.5f;
const float ACCELERATION = 10.0f;
const float PLAYERHEIGHT = 1.7f;
const float GROUNDFRICTION = 0.4f;
const float AIRFRICTION = 0.95f;
const float AIRDENSITY = 1.225f;
const float DRAGCOOEFICIENT = 0.6f;
const float HUMANSURFACEAREA = 1.0f;
const float HUMANWEIGHT = 80.0f;

PlayerHumanBehavior::PlayerHumanBehavior( Actor* actor, World* world, Player* player ) : 
	PlayerBehavior(actor, world, player)
{

}

PlayerHumanBehavior::~PlayerHumanBehavior()
{

}

bool PlayerHumanBehavior::Update( float dt )
{
	if ( PlayerBehavior::Update(dt) )
		return true;

	KeyStates keyStates = this->zPlayer->GetKeys();
	Vector3 curPosition = this->zActor->GetPosition();

	// Movement Keys
	float forwardMovement = (float)(keyStates.GetKeyState(KEY_FORWARD) - keyStates.GetKeyState(KEY_BACKWARD));
	float sideMovement = (float)(keyStates.GetKeyState(KEY_RIGHT) - keyStates.GetKeyState(KEY_LEFT));

	if ( forwardMovement != 0.0f || sideMovement != 0.0f )
	{
		Vector3 moveDir = Vector3(0.0f, 0.0f, 0.0f);

		// Get Directions
		Vector3 playerForwardVector = this->zActor->GetDir();
		playerForwardVector.y = 0; // Need a vector that is horizontal to X and Z
		playerForwardVector.Normalize();
		Vector3 playerRightVector = Vector3(0.0f, 1.0f, 0.0f).GetCrossProduct(playerForwardVector);
		playerRightVector.Normalize();

		// Calc the movement vector
		moveDir += playerForwardVector * forwardMovement;
		moveDir += playerRightVector * sideMovement;
		moveDir.Normalize();
		moveDir *= ACCELERATION;

		// Movement Velocity
		zVelocity += moveDir * dt;
	}

	// Check Ground
	bool isOnGround = false;
	float heightAboveGround = 0.0f;

	try
	{
		float groundHeight = zWorld->CalcHeightAtWorldPos(curPosition.GetXZ());
		heightAboveGround = curPosition.y - groundHeight;
		if( heightAboveGround <= 0.0f ) isOnGround = true;
	}
	catch(...)
	{

	}

	// Gravity
	zVelocity += GRAVITY * dt;

	// Air Resistance
	Vector3 airResistance( pow(zVelocity.x, 2.0f), pow(zVelocity.y, 2.0f), pow(zVelocity.z, 2.0f) );
	if ( zVelocity.x >= 0.0f ) airResistance.x *= -1.0;
	if ( zVelocity.y >= 0.0f ) airResistance.y *= -1.0;
	if ( zVelocity.z >= 0.0f ) airResistance.z *= -1.0;
	airResistance *= 0.5f * AIRDENSITY * DRAGCOOEFICIENT * HUMANSURFACEAREA;
	zVelocity -= airResistance / HUMANWEIGHT * dt;

	// Apply Velocity
	Vector3 newPosition = curPosition + zVelocity * dt;

	try
	{
		float groundHeight = zWorld->CalcHeightAtWorldPos(curPosition.GetXZ());
		if ( newPosition.y < groundHeight ) newPosition.y = groundHeight, zVelocity.y = 0.0f;
		
		// Apply Push
		Vector3 norm = zWorld->CalcNormalAt(newPosition.GetXZ());
		zVelocity += norm * dt;
	}
	catch(...)
	{

	}

	this->zActor->SetPosition(newPosition);

	return false;
}


