#include "Player.h"
#include "ClientData.h"


Player::Player( ClientData* client ) :
	zClient(client),
	zBehavior(0)
{

}

Player::~Player()
{
	zBehavior = NULL;
	zClient = NULL;
}

void Player::Kick()
{
	zClient->Kick();
}
