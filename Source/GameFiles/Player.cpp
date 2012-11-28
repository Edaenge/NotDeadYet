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
D3DXVECTOR3 Player::GetPlayerPosition()
{
	return zPlayerMesh->GetPosition();
}
void Player::AddStaticMesh(StaticMesh* mesh)
{
	this->zPlayerMesh = mesh;
}
D3DXVECTOR3 Player::GetPlayerScale()
{
	return this->zPlayerMesh->GetScaling();
}
D3DXQUATERNION Player::GetPlayerRotation()
{
	return this->zPlayerMesh->GetScaling();
}
int Player::GetClientID()
{
	return this->zClientID;
}
int Player::GetPlayerState()
{
	return this->zPlayerState;
}
StaticMesh* Player::GetPlayerMesh()
{
	return this->zPlayerMesh;
}
void Player::SetClientID(int clientID)
{
	this->zClientID = clientID;
}
void Player::SetPlayerState(int state)
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

