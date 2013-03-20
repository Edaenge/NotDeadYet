#pragma once

#include <World/Observer.h>
#include <map>
#include <KeyStates.h>
#include "ActorManager.h"
#include "ActorSynchronizer.h"
#include "Physics.h"
#include "Item.h"
#include "SoundHandler.h"
#include "MaloWPerformance.h"

class ClientData;
class World;
class GameMode;
class Player;
class Behavior;
class PlayerBehavior;
class ActorSynchronizer;
class KeyStates;
class Entity;
class WorldActor;
class ItemActor;
class AnimalActor;
class CraftingManager;
class MaterialSpawnManager;
class BehaviorManager;
class BerryBushSpawner;

class Game : public Observer, public Observed
{

private:
	MaloWPerformance* zPerf;

	BehaviorManager* zBehaviorManager;
	ActorManager* zActorManager;
	GameMode* zGameMode;
	World* zWorld;
	CraftingManager* zCraftingManager;
	SoundHandler* zSoundHandler;
	PhysicsEngine* zPhysicsEngine;
	ActorSynchronizer* zSyncher;

	// Spawners
	MaterialSpawnManager* zMaterialSpawnManager;
	BerryBushSpawner* zBerryBushSpawner;

	std::map<std::string, Vector3> zCameraOffset;
	std::map<ClientData*, Player*> zPlayers;
	std::map<Entity*, WorldActor*> zWorldActors;
	std::map<std::string, std::string> zPlayerModels;
	std::map<std::string, std::string> zDeadActorModels;
	unsigned int zMaxNrOfPlayers;

	Vector3 zCurrentSunPosition;
	Vector3 zCurrentSunDirection;
	Vector3 zMapCenter;

	float zSunTimer;
	/*! How many total radians the sun has rotated.*/
	float zTotalSunRadiansShift;
	float zSunRadiansShiftPerUpdate;

	/*! Starting Radius of the Fog without players.*/
	float zInitalFogEnclosement;
	/*! How much the fog radius change with each player alive.*/
	float zIncrementFogEnclosement;
	/*! Fog radius that is sent to the clients.*/
	float zCurrentFogEnclosement;
	/*! How often the fog is updated.*/
	float zFogUpdateDelay;
	/*! How much the percent decrease each update.*/
	float zFogDecreaseCoeff;
	/*! How Many percent the fog decreased.*/
	float zFogTotalDecreaseCoeff;
	/*! Counter that counts up until next fog update.*/
	float zFogTimer;

public:
	Game(const int maxClients, PhysicsEngine* physics, ActorSynchronizer* syncher, const std::string& mode, const std::string& worldFile);
	virtual ~Game();

	// Returns false if game has finished
	bool Update( float dt );

	// Event input
	void OnEvent( Event* e );

	// Sets The Behavior Of Player
	void SetPlayerBehavior( Player* player, PlayerBehavior* behavior );

	/*! Converts an item to ItemActor*/
	ItemActor* ConvertToItemActor(Behavior* behavior,  Actor*& oldActorOut);
	
	// Spawn Points
	Vector3 CalcPlayerSpawnPoint(int MaxPoints, Vector2 center);
	Vector3 CalcPlayerSpawnPoint(int nr);
	Vector3 CalcPlayerSpawnPoint(int currentPoint, int maxPoints, float radius, Vector3 center);
	
	// Managers
	inline BehaviorManager* GetBehaviorManager() const { return zBehaviorManager; }
	inline ActorManager* GetActorManager() const { return this->zActorManager; }
	inline World* GetWorld() const { return this->zWorld;}
	inline SoundHandler* GetSoundHandler() const { return this->zSoundHandler; }

	inline void SetPerformancePtr(MaloWPerformance* perf) { this->zPerf = perf; }

	// Send Message To All Players
	void SendToAll(const std::string& msg);

	// Restart Game Logic
	void RestartGame();	

	//Return tru if the server is full.
	bool IsFull() const;

	// Current Fog Enclosement
	inline float GetFogEnclosement() const { return this->zCurrentFogEnclosement; }

	// Player model Offset	
	Vector3 GetOffset(const std::string& model);
	void CheckPlayerUseBow(Player* player);
	void CheckToShotArrow(ClientData* cd);
	const std::map<std::string, std::string>& GetPlayerModels() const {return this->zPlayerModels;}
	const std::map<std::string, Vector3> GetCameraOffsets() const {return this->zCameraOffset;}
	const std::map<std::string, std::string> GetDeadActorModels() const {return this->zDeadActorModels;}
private:
	//Test functions, spawns items/Animals
	void SpawnItemsDebug();
	void SpawnAnimalsDebug();
	void SpawnHumanDebug();

	// Handle Incoming Messages
	void HandleConnection(ClientData* cd);
	void HandleDisconnect(ClientData* cd);
	void HandleLootObject(ClientData* cd, std::vector<unsigned int>& actorID);
	void HandleLootItem(ClientData* cd, unsigned int itemID, unsigned int itemType, unsigned int objID, unsigned int subType );
	void HandleDropItem(ClientData* cd, unsigned int objectID);
	bool HandleUseItem(ClientData* cd, unsigned int itemID);
	void HandleUseWeapon(ClientData* cd, unsigned int itemID);
	bool HandleCraftItem(ClientData* cd, const unsigned int itemType, const unsigned int itemSubType);
	bool HandleFillItem(ClientData* cd, const unsigned int itemID);
	void HandleDrinkWater(ClientData* cd);
	void HandleEquipItem(ClientData* cd, unsigned int itemID);
	void HandleUnEquipItem(ClientData* cd, unsigned int itemID);
	void HandleBindings(const unsigned int ID, const std::string& model, const unsigned int type, const unsigned int subType);

	// Game Stuff
	void UpdateSunDirection(float dt);
	void UpdateFogEnclosement(float dt);
	void Caching(const std::string& modelName);
	void PrintDebugData(ClientData* cd, int type);
	void ResetFogEnclosement();
	void ResetSunDirection();
};