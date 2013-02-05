#pragma once

#include "GameMode.h"


class GameModeFFA : public GameMode
{
public:
	GameModeFFA(World* worldPtr);
	virtual ~GameModeFFA();
private:
	virtual void Update(float dt);
	virtual void WinningCheck();
	virtual bool InitiGameObjects();

	void InitAnimals();
	void InitStaticObjects();
private:

};