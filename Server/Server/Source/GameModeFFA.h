#pragma once

#include "GameMode.h"


class GameModeFFA : public GameMode
{
public:
	GameModeFFA();
	virtual ~GameModeFFA();

private:
	virtual void OnEvent( Event* e );	
};