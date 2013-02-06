#include "Player.h"
#include "ClientData.h"


Player::Player( ClientData* client ) :
	zClient(client)
{

}

Player::~Player()
{

}

void Player::Kick()
{
	zClient->Kick();
}
