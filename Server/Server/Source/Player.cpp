#include "Player.h"
#include "ClientData.h"


Player::Player( ClientData* client ) :
	zClient(client),
	zBehavior(0)
{
	bool zReady = false;
}

Player::~Player()
{
}

void Player::Kick()
{
	zClient->Kick();
}
