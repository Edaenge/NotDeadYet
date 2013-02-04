/*
Made by Edänge Simon 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "ServerListener.h"
#include "ActorHandler.h"
#include "ClientData.h"
#include "World.h"
#include "GameMode.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <vld.h>
#define INCLUDE_MODEL_VIEWER
#endif

class Host : public MaloW::Process, public Observer	
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	const char* InitHost(const unsigned int &port, const unsigned int &maxClients);
	/*! Main loop for this thread*/
	void Life();
	/*! Checks if the server have players connected.*/
	bool HasClients() const;
	/*! Returns the port*/
	inline int GetPort() const{return this->zPort;}
	/*! Sends a message to all connected clients.
		If sendIM is true, it will send a Important message to the client(s).
	*/
	void SendToAllClients(const std::string& message, bool sendIM = false);
	/*! Sends to a specific client.
		If sendIM is true, it will send a Important message to the client(s).
	*/
	void SendToClient(int clientID, const std::string& message, bool sendIM = false);
	void SendToClient(ClientData* cd, const std::string& message, bool sendIM = false);
	/*! Sends Player Actor updates to clients.*/
	void SendPlayerActorUpdates();
	/*! Sends Animal Actor updates to clients.*/
	void SendAnimalActorUpdates();
	/*! Sends Static Actor updates to clients.*/
	void SendStaticActorUpdates();
	/*! Sends Dynamic Actor updates to clients.*/
	void SendDynamicActorUpdates();
	/*! Sends new Static Object Data to Clients*/
	void SendNewObjectMessage(StaticObjectActor* staticObj);
	/*! Sends new Dynamic Object Data to Clients*/
	void SendNewObjectMessage(DynamicObjectActor* dynamicObj);
	/*! Sends new Animal Object Data to Clients*/
	void SendNewObjectMessage(AnimalActor* animalObj);
	/*! Sends a Message To the Client.*/
	void SendErrorMessage(const long ID, const std::string error_Message);
	/*! Notifies all clients, the server is shutting down.*/
	void BroadCastServerShutdown();
	/*! Pings the clients.*/
	void PingClients();
	/*! Updates the server clock.*/
	float Update();
	/*! Updates the objects.*/
	void UpdateObjects(); //MOVE
	/*! Kicks client. Sends a message if reason is given.
		If sendAMessage is false, the client will not be notified.
		This function notifies all the other clients to remove this player.
	*/
	bool KickClient(const int ID, bool sendAMessage = false, std::string reason = "");
	/*! Checks if the server is alive.*/
	bool IsAlive() const;
	/*! */
	int GetNrOfPlayers() const;

private:
	/*! Creates all Animal/Static And Dynamic Objects in the Game At the Beginning of the Game.*/
	void Init();
	/*! Handles new incoming connections.*/
	void HandleNewConnection( MaloW::ClientChannel* CC );
	/*! Handles messages from clients. This function will call the following functions:
	HandleCloseConnectionMsg
	HandleKeyPress
	HandleKeyRelease
	CreateNewPlayer
	*/
	void HandleReceivedMessage( const unsigned int &ID, const std::string &message );
	/*! Read messages from queue and saves them in*/
	void ReadMessages(); 
	/*! Handles clients key press.*/
	void HandleKeyPress(PlayerActor* pl, const std::string& key); //MOVE
	/*! Handles clients key releases.*/
	void HandleKeyRelease(PlayerActor* pl, const std::string& key);  //MOVE
	/*! Handles incoming data from player, such as Direction, Up vector and Rotation.*/
	void HandlePlayerUpdate(PlayerActor* pl, ClientData* cd, const std::vector<std::string> &data);
	/*! Handles the players Important messages, updates them.
		This function will call HandleNackIM / client.
	*/
	void HandleClientNackIM();
	/*! Search for a client. Returns -1 if none was found.*/
	int SearchForClient(const int ID) const;
	/*! Creates a new player and notifies all clients.*/
	void CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data); 
	/*! Returns an Array Containing Existing Static Objects Messages.*/
	void GetExistingObjects(std::vector<std::string>& static_Objects);
	/*! Called When player Disconnects or Dies.*/
	void OnBioActorRemove(BioActor* actor ); //MOVE
	void OnBioActorDeath(BioActor* actor);	//MOVE
	void SendStartMessage();	//DELTE
	//3x3 center = 54,0,54, Map_xxxx center = 1900, 0, 1900
	Vector3 CalculateSpawnPoint(int currentPoint, int maxPoints, float radius, Vector3 center = Vector3(54, 0.0f, 44)); //MOVE
	/*! Temporary function.*/
	void RespawnPlayer(PlayerActor* pActor); //MOVE

	void RemovePlayer(unsigned int ID); //MOVE

	void HandleDisconnect( MaloW::ClientChannel* channel );
	//////////////////////////////////////
	//									//
	//	   Objects/Items Conversions	//
	//			  						//
	//////////////////////////////////////

	/*! Creates an Object From the Item Data.*/
	bool CreateObjectFromItem(PlayerActor* pActor, Weapon* weapon_Item);
	bool CreateObjectFromItem(PlayerActor* pActor, Food* food_Item);
	bool CreateObjectFromItem(PlayerActor* pActor, Container* weapon_Item);
	bool CreateObjectFromItem(PlayerActor* pActor, Projectile* projectile_Item);
	bool CreateObjectFromItem(PlayerActor* pActor, Material* material_Item);
	/*! Creates An Item From the Object Data.*/
	bool CreateItemFromObject(PlayerActor* pActor, WeaponObject* weaponObj);
	bool CreateItemFromObject(PlayerActor* pActor, FoodObject* foodObj);
	bool CreateItemFromObject(PlayerActor* pActor, ContainerObject* containerObj);
	bool CreateItemFromObject(PlayerActor* pActor, StaticProjectileObject* projectileObj);
	bool CreateItemFromObject(PlayerActor* pActor, MaterialObject* materialObj);
	/*! Create Animal Actors*/
	bool CreateAnimalActor(DeerActor** deerAct, const bool genID);
	bool CreateAnimalActor(WolfActor** deerAct, const bool genID);
	/*! Creates a StaticObject with default values.*/
	bool CreateStaticObjectActor(const int type, FoodObject** foodObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, WeaponObject** weaponObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, MaterialObject** materialObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, ContainerObject** containerObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, StaticProjectileObject** projectileObj, const bool genID = false);
	/*! Creates a DynamicObject with default values.*/
	bool CreateDynamicObjectActor(const int type, DynamicProjectileObject** projectileObj, bool genID = false);
	/*! Creates a Dead Player*/
	std::string CreateDeadPlayerObject(PlayerActor* pActor, DeadPlayerObjectActor** dpoActor);
	/*! Creates a Dead Animal*/
	std::string CreateDeadAnimalObject(AnimalActor* aAnimal, DeadAnimalObjectActor** daoActor);

	void CreateNewGhostActor(PlayerActor* pActor);

	void HandleConversion(DynamicProjectileObject* dynamicProjObj);
	
	//////////////////////
	//					//
	//		Items		//
	//			  		//
	//////////////////////

	void SendAddInventoryItemMessage(const int PlayerID, StaticProjectileObject* projectileObj);
	void SendAddInventoryItemMessage(const int PlayerID, FoodObject* foodObj);
	void SendAddInventoryItemMessage(const int PlayerID, ContainerObject* containerObj);
	void SendAddInventoryItemMessage(const int PlayerID, WeaponObject* weaponObj);
	void SendAddInventoryItemMessage(const int PlayerID, Projectile* projectile);
	void SendAddInventoryItemMessage(const int PlayerID, Weapon* weapon);
	void SendAddInventoryItemMessage(const int PlayerID, MaterialObject* material);
	void SendAddInventoryItemMessage(const int PlayerID, Container* container);
	void SendAddInventoryItemMessage(const int PlayerID, Food* food);
	void SendAddInventoryItemMessage(const int PlayerID, Material* material);
	bool HandlePickupItem(PlayerActor* pActor, const long ObjectId);
	void HandleDropItem(PlayerActor* pActor, const long ItemID);
	void HandleItemUse(PlayerActor* pActor, const long ItemID);
	bool UnEquipEquipment(PlayerActor* pActor, Equipment* eq, Inventory* inv, const int itemType);
	void HandleWeaponUse(PlayerActor* pActor, const long ItemID);
	void HandleUnEquipItem(PlayerActor* pActor, const long ItemID, const int Slot);
	bool HandleLootItem(PlayerActor* pActor, const int deadPlayerID, const long id, const int type);

	Item* CreateItemFromDefault(const int ItemType);

	/*! Sends UnEquip Message To Client*/
	void SendUnEquipMessage(const int PlayerID, const long ID, const int Slot);
	void SendWeaponUseMessage(const int PlayerID, const long ID);
	void SendRemoveItemMessage(const int PlayerID, const long ID, const int Slot);
	void SendRemoveItemMessage(const int PlayerID, const long ID);
	void SendRemoveDeadPlayerItem(const int PlayerID, const long ObjID, const long ItemID, const int Type);
	void SendEquipMessage(const int PlayerID, const long ID, const int Slot);
	void SendUseItem(const int PlayerID, const long ID);

	std::string AddItemMessage(StaticObjectActor* object);

protected:
	virtual void OnEvent(Event* e);

private:
	ServerListener* zServerListener;

	std::vector<ClientData*>			zClients;
	std::vector<MaloW::NetworkPacket*>	zMessages;

	ActorHandler* zActorHandler;
	
	NetworkMessageConverter zMessageConverter;

	unsigned int zMaxClients;
	unsigned int zMinClients;
	int zPort;
	bool zGameStarted;
	INT64 zStartime;
	float zSecsPerCnt;
	float zDeltaTime;
	float zTimeOut;
	float zPingMessageInterval;

	World* zWorld;
	std::map<BioActor*, WorldAnchor*> zAnchorPlayerMap;

};