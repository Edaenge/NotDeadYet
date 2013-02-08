#pragma once

#include "GameMode.h"


class GameModeFFA : public GameMode
{
public:
	GameModeFFA(Game* game);
	virtual ~GameModeFFA();

private:
	virtual void OnEvent( Event* e );	
};