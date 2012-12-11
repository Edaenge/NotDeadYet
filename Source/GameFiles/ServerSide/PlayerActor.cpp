#include "GameFiles/ServerSide/PlayerActor.h"

PlayerActor::PlayerActor( const int ID ) : BioActor(ID)
{
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}

PlayerActor::PlayerActor( const int ID, const Vector3& startPos ) : BioActor(ID, startPos)
{
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}

PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot ) : BioActor(ID, startPos, startRot)
{
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::Update(float deltaTime)
{
	if(this->zKeyStates.GetKeyState(KEY_SPRINT))
	{
		if (zState != STATE_RUNNING)
		{
			this->zState = STATE_RUNNING;
		}
		else
		{
			this->zState = STATE_WALKING;
		}
	}
	if(this->zKeyStates.GetKeyState(KEY_DUCK))
	{
		if (zState != STATE_CROUCHING)
		{
			this->zState = STATE_CROUCHING;
		}
		else
		{
			this->zState = STATE_WALKING;
		}
	}
	float dt = deltaTime + this->zLatency;

	this->zFrameTime += dt;
	switch (this->zState)
	{
	case STATE_WALKING:
		this->zVelocity = V_WALK_SPEED;
		break;
	case STATE_RUNNING:
		this->zVelocity = V_RUN_SPEED;
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
}