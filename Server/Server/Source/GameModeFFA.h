#pragma once

#include "GameMode.h"

class GhostActor;
class SupplyDrop;

class GameModeFFA : public GameMode
{
private:
	std::set<Player*> zPlayers;
	SupplyDrop* zSupplyDrop;
	bool zGameStarted;
	bool zGameEnd;

public:
	GameModeFFA(Game* game);
	virtual ~GameModeFFA();

	bool Update(float dt);
	bool HasGameStarted() const {return this->zGameStarted;}
	bool StartGameMode();

private:
	virtual void OnEvent( Event* e );
	void OnPlayerHumanDeath(PlayerActor* pActor);
	void OnPlayerAnimalDeath(AnimalActor* aActor);
	void SwapToAnimal(GhostActor* actor, unsigned int animalType);
	void PossessAnAnimal(GhostActor* actor);
	
};