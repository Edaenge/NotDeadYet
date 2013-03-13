#pragma once

#include <World/Observer.h>
#include "Player.h"

class Game;
class ActorManager;


class SoundHandler : public Observer
{
private:
	std::set<Player*> zPlayers;
	Game* zGame;
	ActorManager* zActorManager;

public:
	SoundHandler(Game* game, ActorManager* actors);
	virtual ~SoundHandler();

protected:
	void OnEvent(Event* e);
};