#include "Player.h"

Player::Player()
{
	this ->zPlayerState = IDLE;
	this->zPlayerMesh = NULL;
}
Player::~Player()
{
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

