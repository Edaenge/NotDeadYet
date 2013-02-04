
#pragma once

#include "ActorHandler.h"
#include "ObjectManager.h"
#include "NetworkPacket.h"
#include "NetworkMessageConverter.h"

static const enum GAMEMODE
{
	GAMEMODE_FFA
};

//This class is abstract

class GameMode
{
public:
	GameMode();
	virtual ~GameMode();

	virtual void Update(float dt) = 0;
	virtual void WinningCheck() = 0;
	virtual void RestartGame();
	virtual void CalculateSpawnPoint();
	virtual void CreatePlayer(long player_ID, const std::vector<std::string> &data);
	virtual bool InitiGameObjects() = 0;

	void HandleKeyPress(long player_ID, const int key);
	void HandleKeyRelease(long player_ID, const int key);

	bool IsRunning() const {return zGameRunning;}


protected:
	ObjectManager* zObjectManager;
	ActorHandler* zActorHandler;
	std::vector<MaloW::NetworkPacket*> zNetworkMessages;

	bool zGameRunning;

};
