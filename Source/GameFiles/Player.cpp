#include "Player.h"

Player::Player()
{
	this->zPlayerState = IDLE;
	this->zPlayerMesh = NULL;
}
Player::~Player()
{
	//this->zPlayerMesh = NULL;
	//SAFE_DELETE(this->zPlayerMesh);
}
const D3DXVECTOR3 Player::GetPlayerPosition()
{
	return zPlayerMesh->GetPosition();
}
void Player::AddStaticMesh(StaticMesh* mesh)
{
	this->zPlayerMesh = mesh;
}
const D3DXVECTOR3 Player::GetPlayerScale()
{
	return this->zPlayerMesh->GetScaling();
}
const D3DXQUATERNION Player::GetPlayerRotation()
{
	return this->zPlayerMesh->GetScaling();
}
const int Player::GetClientID()
{
	return this->zClientID;
}
const int Player::GetPlayerState()
{
	return this->zPlayerState;
}
StaticMesh* Player::GetPlayerMesh()
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
void Player::SetPlayerPosition(D3DXVECTOR3 pos)
{
	this->zPlayerMesh->SetPosition(pos);
}
void Player::SetPlayerScale(D3DXVECTOR3 scale)
{
	this->zPlayerMesh->Scale(scale);
}
void Player::SetPlayerRotation(D3DXQUATERNION rot)
{
	this->zPlayerMesh->SetQuaternion(rot);
}

