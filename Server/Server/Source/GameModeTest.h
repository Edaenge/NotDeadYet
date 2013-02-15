#pragma once

#include "GameMode.h"

class GameModeTest : public GameMode
{
private:
	int zKillLimit;
	std::map<Player*, int> zScoreBoard;
	std::set<Player*> zPlayers;
public:
	GameModeTest(Game* game, int killLimit);
	virtual ~GameModeTest();

	bool Update(float dt);

private:
	virtual void OnEvent( Event* e );
	void OnPlayerDeath(PlayerActor* pActor);
};