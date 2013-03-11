#pragma once

#include <World/Observer.h>
#include "PlayerActor.h"

class GameMode : public Observer
{
protected:
	Game* zGame;

	
public:
	GameMode(Game* game) : zGame(game) {};
	virtual ~GameMode() {};

	virtual bool Update(float) = 0;
	virtual void OnEvent(Event* e) = 0;
};