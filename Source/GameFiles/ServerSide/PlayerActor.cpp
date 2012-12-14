#include "GameFiles/ServerSide/PlayerActor.h"

PlayerActor::PlayerActor( const int ID ) : BioActor()
{
	InitValues();
	this->zID = ID;
}

PlayerActor::PlayerActor( const int ID, const Vector3& startPos ) : BioActor(startPos)
{
	InitValues();
	this->zID = ID;
}

PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot ) : BioActor(startPos, startRot)
{
	InitValues();
	this->zID = ID;
}

void PlayerActor::InitValues()
{
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::Update(float deltaTime)
{
	float dt = deltaTime + this->zLatency;

	if(this->zKeyStates.GetKeyState(KEY_SPRINT))
	{
		if(Sprint(dt))
			this->zState = STATE_RUNNING;
		else
			this->zState = STATE_WALKING;
	}

	else if(this->zKeyStates.GetKeyState(KEY_DUCK))
		this->zState = STATE_CROUCHING;

	else
		this->zState = STATE_WALKING;
	
	this->zFrameTime += dt;
	switch (this->zState)
	{
	case STATE_WALKING:
		this->zVelocity = V_WALK_SPEED;
		break;
	case STATE_RUNNING:
		this->zVelocity = V_RUN_SPEED;
		break;
	case STATE_CROUCHING:
		this->zVelocity = V_CROUCH_SPEED;
		break;
	case STATE_IDLE:
		break;
	default:
		break;
	}

	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
	{
		this->zPos = this->zPos + this->zDir * dt * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		this->zPos = this->zPos + this->zDir * -1 * dt * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * dt * this->zVelocity);
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * -1 * dt * this->zVelocity);
	}

	if(this->zState != STATE_RUNNING && (this->zStamina != this->zStaminaMax))
	{
		this->zStamina += dt * this->zStaminaCof;

		if(this->zStamina > this->zStaminaMax)
			this->zStamina = this->zStaminaMax;
	}
}