/*
Made by Edänge Simon 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "../Network/ServerListener.h"
#include "../../../../../Source/Network/NetworkMessageConverter.h"
#include "PlayerActor.h"
#include "ActorHandler.h"

#if defined(DEBUG) || defined(_DEBUG)
	#include <vld.h>
	#define INCLUDE_MODEL_VIEWER
#endif

struct ClientData
{
	ClientData(MaloW::ClientChannel* cc)
	{
		zClient = cc;
		zPinged = false;
		zCurrentPingTime = 0.0f;
		zTotalPingTime = 0.0f;
		zNrOfPings = 0;
	}

	~ClientData()
	{
		SAFE_DELETE(zClient);
	}

	inline void IncPingTime(float dt)
	{
		zCurrentPingTime += dt;
	}
	inline void ResetPingCounter()
	{
		zPinged = 0;
		zTotalPingTime = 0.0f;
	}

	bool zPinged;
	float zCurrentPingTime;
	float zTotalPingTime;
	int zNrOfPings;
	MaloW::ClientChannel* zClient;

};

class Host : public MaloW::Process
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	int InitHost(const int PORT, const unsigned int MAX_CLIENTS);
	/*! Main loop for this thread*/
	void Life();
	/*! Checks if the server have players connected.*/
	bool HasClients() const;
	/*! Returns the port*/
	inline int GetPort() const{return this->zPort;}
	/*! Sends a message to all connected clients.*/
	void SendToAllClients(const std::string& message);
	/*! Sends to a specific client.*/
	void SendToClient(int clientID, const std::string& message);
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
	void SendErrorMessage(const int ID, const std::string error_Message);
	/*! Sends UnEquip Message To Client*/
	void SendUnEquipMessage(const int PlayerID, const int ID, const int Slot);
	/*! Notifies all clients, the server is shutting down.*/
	void BroadCastServerShutdown();
	/*! Pings the clients.*/
	void PingClients();
	/*! Updates the server clock.*/
	float Update();
	/*! Updates the objects.*/
	void UpdateObjects();
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
	void HandleNewConnections();
	/*! Handles messages from clients. This function will call the following functions:
	HandlePingMsg
	HandleCloseConnectionMsg
	HandleKeyPress
	HandleKeyRelease
	CreateNewPlayer
	*/
	void HandleRecivedMessages();
	/*! Read messages from queue and saves them in*/
	void ReadMessages(); 
	/*! Handles ping messages.*/
	void HandlePingMsg(ClientData* cd);
	/*! Handles clients key press.*/
	void HandleKeyPress(PlayerActor* pl, const std::string& key);
	/*! Handles clients key releases.*/
	void HandleKeyRelease(PlayerActor* pl, const std::string& key);
	/*! Handles incoming data from player, such as Direction, Up vector and Rotation.*/
	void HandlePlayerUpdate(PlayerActor* pl, ClientData* cd, const std::vector<std::string> &data);
	/*! Search for a client. Returns -1 if none was found.*/
	int SearchForClient(const int ID) const;
	/*! Creates a new player and notifies all clients.*/
	void CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data);
	bool HandlePickupItem(PlayerActor* pActor, const int ObjectId);
	void HandleDropItem(PlayerActor* pActor, const int ItemID);
	void HandleItemUse(PlayerActor* pActor, const int ItemID);
	void HandleWeaponUse(PlayerActor* pActor, const int ItemID);
	void HandleUnEquipItem(PlayerActor* pActor, const int ItemID, const int Slot);
	/*! Returns an Array Containing Existing Static Objects Messages.*/
	void GetExistingObjects(std::vector<std::string>& static_Objects);
	/*! Creates an Object From the Item Data.*/
	bool CreateObjectFromItem(PlayerActor* pActor, Weapon* weapon_Item);
	bool CreateObjectFromItem(PlayerActor* pActor, Food* food_Item);
	bool CreateObjectFromItem(PlayerActor* pActor, Container* weapon_Item);
	/*! Creates An Item From the Object Data.*/
	bool CreateItemFromObject(PlayerActor* pActor, WeaponObject* weaponObj);
	bool CreateItemFromObject(PlayerActor* pActor, FoodObject* foodObj);
	bool CreateItemFromObject(PlayerActor* pActor, ContainerObject* containerObj);
	bool CreateItemFromObject(PlayerActor* pActor, StaticProjectileObject* projectileObj);
	/*! Creates a StaticObject with default values.*/
	bool CreateStaticObjectActor(const int type, WeaponObject** weaponObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, FoodObject** foodObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, ContainerObject** containerObj, const bool genID = false);
	bool CreateStaticObjectActor(const int type, StaticProjectileObject** projectileObj, const bool genID = false);
	/*! Creates a DynamicObject with default values.*/
	bool CreateDynamicObjectActor(const int type, DynamicProjectileObject** projectileObj, bool genID = false);
	void HandleConversion(DynamicProjectileObject* dynamicProjObj);
	void SendRemoveEquipment(const int ClientID, const int ItemID, const int Slot);
	Item* CreateItemWithDefaultValues(const int ItemType);
private:
	ServerListener* zServerListener;

	std::vector<ClientData*>			zClients;
	std::vector<MaloW::NetworkPacket*>	zMessages;

	ActorHandler* zActorHandler;
	
	NetworkMessageConverter zMessageConverter;

	unsigned int zMaxClients;
	int zPort;
	
	INT64 zStartime;
	float zSecsPerCnt;
	float zDeltaTime;
	float zTimeOut;
	float zPingMessageInterval;
};