#pragma once

#include <Observer.h>

class GameMode : public Observer
{
protected:
	Game* zGame;

	GameMode(Game* game) : zGame(game) {};
	virtual ~GameMode() {};
public:

	virtual bool Update(float) = 0;
	virtual void OnEvent(Event* e) = 0;
};