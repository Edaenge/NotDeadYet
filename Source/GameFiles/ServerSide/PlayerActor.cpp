#include "GameFiles/ServerSide/PlayerActor.h"


PlayerActor::PlayerActor( const int ID ) : Actor()
{
	this->zID = ID;

	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zPos = Vector3(1,0,1);
	this->zDir = Vector3(1, 0, 0);
	this->zScale = Vector3(0.05f,0.05f,0.05f);
	this->zPlayerModel = "none";
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos ) : Actor(startPos, Vector4())
{
	this->zID = ID;
	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zDir = Vector3(1, 0, 0);
	this->zScale = Vector3(0.05f,0.05f,0.05f);
	this->zPlayerModel = "none";
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot ) : Actor(startPos, startRot)
{
	this->zID = ID;
	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zDir = Vector3(1, 0, 0);
	this->zScale = Vector3(0.05f,0.05f,0.05f);
	this->zPlayerModel = "none";
}


PlayerActor::~PlayerActor()
{

}


void PlayerActor::Update(float deltaTime)
{
	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
	{
		this->zPos = this->zPos + this->zDir * deltaTime * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_BACKWARD))
	{
		this->zPos = this->zPos + this->zDir * -1 * deltaTime * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = up.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + right * -1 * deltaTime * this->zVelocity;
	}
	if(this->zKeyStates.GetKeyState(KEY_LEFT))
	{
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = up.GetCrossProduct(this->zDir);
		this->zPos = this->zPos + right * deltaTime * this->zVelocity;
	}
}