#pragma once

#include <World/Observer.h>
#include "PlayerActor.h"

class GameMode : public Observer
{
protected:
	Game* zGame;
	bool zGameStarted;
	bool zGameEnd;

public:
	GameMode(Game* game) : 
		zGame(game) 
	{
		if ( zGame ) zGame->AddObserver(this);

		this->zGameStarted = false;
		this->zGameEnd = false;
	};

	virtual ~GameMode() 
	{
		if ( zGame ) zGame->RemoveObserver(this);
	};

	virtual bool IsGameStarted() const {return this->zGameStarted;}
	virtual bool IsGameEnded() const {return this->zGameEnd;}
	virtual bool Update(float) = 0;
	virtual void OnEvent(Event* e) = 0;
	virtual bool StartGameMode() = 0;
	virtual bool StopGameMode() = 0;
	virtual bool CanConnect( ClientData* ) { return true; }
};