#include "GameFiles/ServerSide/PlayerActor.h"


PlayerActor::PlayerActor( const int ID ) : Actor()
{
	this->zID = ID;

	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zPos = Vector3(1,0,1);
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zPlayerModel = "none";
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos ) : Actor(startPos, Vector4())
{
	this->zID = ID;
	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zPlayerModel = "none";
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot ) : Actor(startPos, startRot)
{
	this->zID = ID;
	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zPlayerModel = "none";
}


PlayerActor::~PlayerActor()
{

}


void PlayerActor::Update(float deltaTime)
{
	if(this->zKeyStates.GetKeyState(KEY_FORWARD))
	{
		//this->zPos = 
	}
}