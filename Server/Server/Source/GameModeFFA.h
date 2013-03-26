#pragma once

#include "GameMode.h"

class GhostActor;
class SupplyDrop;

struct DeadActors
{
	BioActor* bActor;
	float time;
};

class GameModeFFA : public GameMode
{
private:
	std::set<Player*> zPlayers;
	std::vector<Actor*> zDeadActors;
	SupplyDrop* zSupplyDrop;
	float zCurrentRSPTime;
	unsigned int zAlivePlayers;
	std::vector<DeadActors> zToBeDeadActors;
public:
	GameModeFFA(Game* game);
	virtual ~GameModeFFA();

	bool Update(float dt);
	bool SpawnRandomDrop();
	virtual bool StartGameMode();
	virtual bool StopGameMode();
	virtual bool CanConnect( ClientData* cd );
	//Checks if the game has ended, returns true if it has ended.
	bool CheckEndCondition();
	unsigned int GetNrOfAlivePlayers() const {return this->zAlivePlayers;}

private:
	std::set<Item*> GenerateItems();
	virtual void OnEvent( Event* e );
	void OnPlayerHumanDeath(PlayerActor* pActor);
	void OnPlayerAnimalDeath(AnimalActor* aActor);
	void SwapToAnimal(GhostActor* actor, unsigned int animalType);
	void PossessAnAnimal(GhostActor* actor);
	
};