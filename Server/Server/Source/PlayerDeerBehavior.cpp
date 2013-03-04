
#include "PlayerDeerBehavior.h"
#include <World/World.h>
#include "Player.h"
#include "Actor.h"
#include "PlayerActor.h"

const float MAX_VELOCITY_WALK = 1.20f;
const float MAX_VELOCITY_RUN = 4.40f;
const float MAX_VELOCITY_SPRINT = 7.80f;
const float MAX_VELOCITY_DUCK = 0.1f;
const Vector3 GRAVITY = Vector3(0, -9.82f, 0);
//const Vector3 JUMPING_FORCE = Vector3(0, 2.84f, 0);
const float ELASTICITY = 0.5f;
const float ACCELERATION = 10000.0f;
const float GROUNDFRICTION = 0.4f;
const float AIRFRICTION = 0.95f;
const float AIRDENSITY = 1.225f;
const float DRAGCOOEFICIENT = 0.6f;
const float DEERSURFACEAREA = 1.8f;
const float DEERWEIGHT = 110.0f;

PlayerDeerBehavior::PlayerDeerBehavior( Actor* actor, World* world, Player* player) : 
	PlayerBehavior(actor, world, player)
{

}

PlayerDeerBehavior::~PlayerDeerBehavior()
{

}


bool PlayerDeerBehavior::Update( float dt )
{
	/*KeyStates keyStates = this->zPlayer->GetKeys();
	Vector3 newPlayerPos;
	Vector3 moveDir = Vector3(0.0f, 0.0f, 0.0f);

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
	moveDir += currentPlayerRight * (float)(keyStates.GetKeyState(KEY_RIGHT) - keyStates.GetKeyState(KEY_LEFT));
	moveDir.Normalize();
	moveDir *= 10.0f;

	this->zVelocity += (moveDir + GRAVITY) * dt;

	if(this->zVelocity.GetLength() > MAX_VELOCITY)
	{
		this->zVelocity.Normalize();
		this->zVelocity *= MAX_VELOCITY;
	}

	newPlayerPos = currentPlayerPos + this->zVelocity * dt;

	float newGroundHeight = zWorld->CalcHeightAtWorldPos(newPlayerPos.GetXZ());
	if(newGroundHeight > newPlayerPos.y)
	{
		newPlayerPos.y = newGroundHeight;
		Vector3 newGroundNormal = zWorld->CalcNormalAt(newPlayerPos.GetXZ());
		this->zVelocity = newGroundNormal * this->zVelocity.GetLength() * ELASTICITY;
	}
	this->zActor->SetPosition(newPlayerPos);
	this->zAnchor->position = newPlayerPos.GetXZ();
	this->zAnchor->radius = SIGHTRADIUS;*/

	if ( !zPlayer )
		return true;

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

	// Check On Ground
	bool isOnGround = false;
	float heightAboveGround = 0.0f;
	bool isJumping = false;

	try
	{
		float groundHeight = zWorld->CalcHeightAtWorldPos(curPosition.GetXZ());
		heightAboveGround = curPosition.y - groundHeight;
		if( heightAboveGround <= 0.0f) 
		{
			isOnGround = true;
			curPosition.y = groundHeight;
			zVelocity.y = 0.0f;
		}
	}
	catch(...)
	{

	}

	// Air Resistance
	Vector3 airResistance(0.0f, 0.0f, 0.0f);
	if ( zVelocity.x != 0.0f ) airResistance.x = powf(zVelocity.x, 2.0f);
	if ( zVelocity.y != 0.0f ) airResistance.y = powf(zVelocity.y, 2.0f);
	if ( zVelocity.z != 0.0f ) airResistance.z = powf(zVelocity.z, 2.0f);

	if ( zVelocity.x >= 0.0f ) airResistance.x *= -1.0;
	if ( zVelocity.y >= 0.0f ) airResistance.y *= -1.0;
	if ( zVelocity.z >= 0.0f ) airResistance.z *= -1.0;
	airResistance *= 0.5f * AIRDENSITY * DRAGCOOEFICIENT * DEERSURFACEAREA;
	zVelocity -= airResistance / DEERWEIGHT * dt;

	//Check if fall/slide
	Vector3 groundNormal = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 normGround = zWorld->CalcNormalAt(curPosition.GetXZ());

	//Calc the vector in the plane.
	Vector3 groundProj = normGround;
	groundProj.y = 0;
	groundProj.Normalize();

	float angle = acos(normGround.GetDotProduct(groundProj));
	angle = angle * TODEGREES;
	if(angle < 45.0f)
	{
		zVelocity = Vector3(0.0f, 0.0f, 0.0f);
		groundNormal = this->zWorld->CalcNormalAt(curPosition.GetXZ()) * 4.0f;
		this->zVelDown = 0.0f;
	}
	else
		zVelocity -= ( zVelocity * 4.0f ) * dt;

	if(!isOnGround)
		this->zVelDown += -9.82f * dt; // Gravity

	BioActor* bActor = dynamic_cast<BioActor*>(zActor);

	// Check Max Speeds
	if(keyStates.GetKeyState(KEY_DUCK))
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_WALK)
		{
			bActor->SetState(STATE_WALKING);
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_WALK;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_WALK)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				bActor->SetState(STATE_IDLE);
				this->zVelocity = Vector3(0, 0, 0);
			}
		}
	}
	else if(!keyStates.GetKeyState(KEY_SPRINT))
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_RUN)
		{
			bActor->SetState(STATE_WALKING);
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_RUN;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_RUN/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				bActor->SetState(STATE_IDLE);
				this->zVelocity = Vector3(0, 0, 0);
			}
		}

	}
	else if(keyStates.GetKeyState(KEY_SPRINT))
	{
		if(this->zVelocity.GetLength() > MAX_VELOCITY_SPRINT)
		{
			bActor->SetState(STATE_RUNNING);
			this->zVelocity.Normalize();
			this->zVelocity *= MAX_VELOCITY_SPRINT;
		}
		else if(this->zVelocity.GetLength() < MAX_VELOCITY_SPRINT/2)
		{
			if( !keyStates.GetKeyState(KEY_FORWARD) && !keyStates.GetKeyState(KEY_BACKWARD) && !keyStates.GetKeyState(KEY_RIGHT) && !keyStates.GetKeyState(KEY_LEFT))
			{
				bActor->SetState(STATE_IDLE);
				this->zVelocity = Vector3(0, 0, 0);
			}
		}

	}

	try
	{
		float groundHeight = zWorld->CalcHeightAtWorldPos(curPosition.GetXZ());
		heightAboveGround = curPosition.y - groundHeight;
		if( heightAboveGround <= 0.0f) 
		{
			isOnGround = true;
			curPosition.y = groundHeight;
			zVelocity.y = 0.0f;
			this->zVelDown = 0.0;
		}
	}
	catch(...)
	{

	}

	// Apply Velocity
	Vector3 newPosition = curPosition + (zVelocity * dt) + Vector3(0.0f, this->zVelDown, 0.0f) + (groundNormal * dt);

	//Look so player isn't outside of World.
	if ( zWorld->IsInside(newPosition.GetXZ()) )
	{
		zActor->SetPosition(newPosition);
	}
	else
	{
		zVelocity = Vector3(0.0f, 0.0f, 0.0f);

		// Move To Center of World
		Vector3 center;
		center.x = zWorld->GetWorldCenter().x;
		center.z = zWorld->GetWorldCenter().y;
		center.y = zWorld->CalcHeightAtWorldPos(Vector2(center.x, center.z));
		zActor->SetPosition(center);
	}

	return false;
}