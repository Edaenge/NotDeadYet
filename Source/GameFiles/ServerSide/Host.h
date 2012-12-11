/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "Network/ServerListener.h"
#include "Network/NetworkMessageConverter.h"
#include "GameFiles/ServerSide/PlayerActor.h"
#include "Safe.h"


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
	int InitHost(int port, unsigned int maxClients);
	/*! Main loop for this thread*/
	void Life();
	/*! Checks if the server have players connected.*/
	bool HasPlayers() const;
	/*! Returns the port*/
	inline int GetPort() const{return this->zPort;}
	/*! Sends a message to all connected clients.*/
	void SendToAllClients(const std::string& message);
	/*! Sends to a specific client.*/
	void SendToClient(int clientID, const std::string& message);
	/*! Sends player updates to clients.*/
	void SendPlayerUpdates();
	/*! Notifies all clients, the server is shutting down.*/
	void BroadCastServerShutdown();
	/*! Pings the clients.*/
	void PingClients();
	/*! Updates the server clock.*/
	float Update();
	/*! Updates the players.*/
	void UpdatePl();
	/*! Kicks client. Sends a message if reason is given.
		If sendAMessage is false, the client will not be notified.
		This function notifies all the other clients to remove this player.
	*/
	bool KickClient(const int ID, bool sendAMessage = false, std::string reason = "");
	/*! Checks if the server is alive.*/
	bool IsAlive() const;

private:
	/*! Handles new incoming connections.*/
	void HandleNewConnections();
	/*! Handles messages from clients. This function will call the following functions:
	HandlePingMsg
	HandleCloseConnectionMsg
	HandleKeyPress
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
	/*! Search for a player. Returns -1 if none was found.*/
	int SearchForPlayer(const int ID) const;
	/*! Creates a new player and notifies all clients.*/
	void CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data);
	
private:
	ServerListener* zServerListener;

	std::vector<ClientData*>			zClients;
	std::vector<PlayerActor*>			zPlayers;
	std::vector<MaloW::NetworkPacket*>	zMessages;
	
	NetworkMessageConverter zMessageConverter;

	unsigned int zMaxClients;
	int zPort;
	
	INT64 zStartime;
	float zSecsPerCnt;
	float zDeltaTime;
	float zTimeOut;
	float zUpdateDelay;
	float zPingMessageInterval;



};