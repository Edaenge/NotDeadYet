#pragma once

#include "GameMode.h"

class GhostActor;

class GameModeFFA : public GameMode
{
private:
	std::set<Player*> zPlayers;

public:
	GameModeFFA(Game* game);
	virtual ~GameModeFFA();

	bool Update(float dt);

private:
	virtual void OnEvent( Event* e );
	void OnPlayerHumanDeath(PlayerActor* pActor);
	void OnPlayerAnimalDeath(AnimalActor* aActor);
	void SwapToAnimal(GhostActor* actor, unsigned int animalType);
	
};