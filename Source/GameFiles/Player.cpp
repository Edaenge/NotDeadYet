#include "Player.h"

Player::Player()
{
	this->zPlayerState = IDLE;
	this->zPlayerMesh = 0;
}


Player::~Player()
{
	//this->zPlayerMesh = NULL;
	//SAFE_DELETE(this->zPlayerMesh);
}


Vector3 Player::GetPlayerPosition() const
{
	return zPlayerMesh->GetPosition();
}


void Player::AddStaticMesh(iMesh* mesh)
{
	this->zPlayerMesh = mesh;
}


Vector3 Player::GetPlayerScale() const
{
	return this->zPlayerMesh->GetScaling();
}


Vector3 Player::GetPlayerRotation() const
{
	// NOTE: GetRotation does not work correctly
	return this->zPlayerMesh->GetRotation();
}


int Player::GetClientID() const
{
	return this->zClientID;
}


int Player::GetPlayerState() const
{
	return this->zPlayerState;
}


iMesh* Player::GetPlayerMesh() const
{
	return this->zPlayerMesh;
}


void Player::SetClientID(const int clientID)
{
	this->zClientID = clientID;
}


void Player::SetPlayerState(const int state)
{
	this->zPlayerState = state;
}


void Player::SetPlayerPosition( const Vector3& pos )
{
	this->zPlayerMesh->SetPosition(pos);
}


void Player::SetPlayerScale( const Vector3& scale )
{
	this->zPlayerMesh->Scale(scale);
}


void Player::SetPlayerRotation( const Vector4& rot )
{
	this->zPlayerMesh->SetQuaternion(rot);
}
