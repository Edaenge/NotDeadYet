#include "GameFiles/ServerSide/PlayerActor.h"


PlayerActor::PlayerActor( const int ID )
{
	this->zID = ID;

	this->zVelocity = 5;
	this->zState = 0;
	this->zPos = Vector3(1,0,1);
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zPlayerModel = "none";
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos )
{
	this->zID = ID;
	this->zPos = startPos;

	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zPlayerModel = "none";
}


PlayerActor::PlayerActor( const int ID, const Vector3& startPos, const Vector4& startRot )
{
	this->zID = ID;
	this->zRot = startRot;

	this->zVelocity = 5;
	this->zState = STATE_IDLE;
	this->zPos = Vector3(1,0,1);
	this->zScale = Vector3(0.05,0.05,0.05);
	this->zPlayerModel = "none";
}


PlayerActor::~PlayerActor()
{

}


void PlayerActor::Update()
{
	
}


int PlayerActor::GetID() const
{
	return this->zID;
}


float PlayerActor::GetVelocity() const
{
	return this->zVelocity;
}


int PlayerActor::GetState() const
{
	return this->zState;
}


const std::string& PlayerActor::GetPlayerModel() const
{
	return this->zPlayerModel;
}


const Vector3& PlayerActor::GetDirection() const
{
	return this->zDir;
}


void PlayerActor::SetState( int state )
{
	this->zState = state;
}


void PlayerActor::SetVelocity( float velocity )
{
	this->zVelocity = velocity;
}


void PlayerActor::SetDirection( const Vector3& dir )
{
	this->zDir = dir;
}


void PlayerActor::SetPlayerModel( const std::string& model )
{
	this->zPlayerModel = model;
}

