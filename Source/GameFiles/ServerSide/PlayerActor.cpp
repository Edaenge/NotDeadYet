#include "GameFiles/ServerSide/PlayerActor.h"


PlayerActor::PlayerActor( const int ID ) : Actor()
{
	this->zID = ID;

	this->zVelocity = V_WALK_SPEED;
	this->zState = STATE_IDLE;

	this->zPos = Vector3(1,0,1);
	this->zDir = Vector3(1, 0, 0);
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zUp	= Vector3(0.0f, 0.0f, 0.0f);

	this->zPlayerModel = "none";
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos ) : Actor(startPos, Vector4())
{
	this->zID = ID;
	this->zVelocity = 5;
	this->zState = STATE_IDLE;

	this->zDir = Vector3(1, 0, 0);
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zUp	= Vector3(0.0f, 0.0f, 0.0f);

	this->zPlayerModel = "none";
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot ) : Actor(startPos, startRot)
{
	this->zID = ID;
	this->zVelocity = 5;
	this->zState = STATE_IDLE;

	this->zDir = Vector3(1, 0, 0);
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zUp	= Vector3(0.0f, 0.0f, 0.0f);

	this->zPlayerModel = "none";
	this->zFrameTime = 0.0f;
	this->zLatency = 0.0f;
}


PlayerActor::~PlayerActor()
{

}


void PlayerActor::Update(float deltaTime)
{
	this->zFrameTime += deltaTime + this->zLatency;

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
		this->zPos = this->zPos + this->zDir * deltaTime * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		this->zPos = this->zPos + this->zDir * -1 * deltaTime * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_RIGHT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * -1 * deltaTime * this->zVelocity);
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 right = this->zUp.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + (right * deltaTime * this->zVelocity);
	}
}