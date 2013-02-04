
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
	GameMode(World* worldPtr);
	virtual ~GameMode();

	virtual void Update(float dt) = 0;
	virtual void WinningCheck() = 0;
	virtual void RestartGame();
	//3x3.map = Vector3(54.0f, 0.0f, 44.0f); large map = Vector3(1900.0f, 0, 1900.0f);
	virtual Vector3 CalculateSpawnPoint(const int point, const int maxPoints, float radius, Vector3 center = Vector3(54.0f, 0.0f, 44.0f));
	virtual void CreatePlayer(long player_ID, const std::vector<std::string> &data);
	virtual bool InitiGameObjects() = 0;

	void HandleKeyPress(long player_ID, const int key);
	void HandleKeyRelease(long player_ID, const int key);

	bool IsRunning() const {return zGameRunning;}

	inline ActorHandler* GetActorHandlerPtr() const {return this->zActorHandler;}
protected:
	ActorHandler* zActorHandler;
	std::vector<MaloW::NetworkPacket*> zNetworkMessages;
	World* zWorld;

	bool zGameRunning;
};
