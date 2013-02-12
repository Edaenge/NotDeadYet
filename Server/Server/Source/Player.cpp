#include "Player.h"
#include "ClientData.h"


Player::Player( ClientData* client ) :
	zClient(client),
	zBehavior(0)
{

}

Player::~Player()
{
	SAFE_DELETE(zBehavior);
}

void Player::Kick()
{
	zClient->Kick();
}
