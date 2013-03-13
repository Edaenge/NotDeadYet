#pragma once

#include <World/Observer.h>
#include "Player.h"

class Game;


class SoundHandler : public Observer
{
private:
	std::set<Player*> zPlayers;
	Game* zGame;

public:
	SoundHandler(Game* game);
	virtual ~SoundHandler();

protected:
	void OnEvent(Event* e);
};