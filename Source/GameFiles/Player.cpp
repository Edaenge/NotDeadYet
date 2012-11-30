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


const Vector3 Player::GetPlayerPosition()
{
	return zPlayerMesh->GetPosition();
}


void Player::AddStaticMesh(iMesh* mesh)
{
	this->zPlayerMesh = mesh;
}


const Vector3 Player::GetPlayerScale()
{
	return this->zPlayerMesh->GetScaling();
}


const Vector3 Player::GetPlayerRotation()
{
	// NOTE: GetRotation does not work correctly
	return this->zPlayerMesh->GetRotation();
}


const int Player::GetClientID()
{
	return this->zClientID;
}


const int Player::GetPlayerState()
{
	return this->zPlayerState;
}


iMesh* Player::GetPlayerMesh()
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


void Player::SetPlayerPosition(Vector3 pos)
{
	this->zPlayerMesh->SetPosition(pos);
}


void Player::SetPlayerScale(Vector3 scale)
{
	this->zPlayerMesh->Scale(scale);
}


void Player::SetPlayerRotation(Vector4 rot)
{
	this->zPlayerMesh->SetQuaternion(rot);
}

