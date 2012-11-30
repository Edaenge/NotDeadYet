/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "Network/ServerListener.h"
#include "Network/NetworkMessageConverter.h"
#include "GameFiles/PlayerActor.h"
#include "Safe.h"


struct ClientData
{
	ClientData(MaloW::ClientChannel* cc)
	{
		zClient = cc;
		zPinged = false;
		zCurrentPingTime = 0.0f;
	}

	~ClientData()
	{
		SAFE_DELETE(zClient);
	}

	void IncPingTime(float dt)
	{
		zCurrentPingTime += dt;
	}

	bool zPinged;
	float zCurrentPingTime;
	MaloW::ClientChannel* zClient;

};

class Host : public MaloW::Process
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	int InitHost(int port, int maxClients);
	/*! Main loop for this thread*/
	void Life();
	/*! Checks if the server have players connected.*/
	bool HasPlayers() const;
	/*! Returns the port*/
	int GetPort() const;
	/*! Sends a message to all connected clients.*/
	void SendToAllClients(const std::string& message);
	/*! Sends to a specific client.*/
	void SendToClient(int clientID, const std::string& message);
	/*! Notifies all clients, the server is shutting down.*/
	void BroadCastServerShutdown();
	/*! Pings the clients.*/
	void PingClients();
	/*! Updates the server clock.*/
	float Update();
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
	/*! Handles ping messages.*/
	void HandlePingMsg(const int CLIENT_ID);
	/*! Handles clients key press.*/
	void HandleKeyPress(const int CLIENT_ID, const std::string& key);
	/*! Search for a client. Returns -1 if none was found.*/
	int SearchForClient(const int ID) const;
	/*! Search for a player. Returns -1 if none was found.*/
	int SearchForPlayer(const int ID) const;
	/*! Creates a new player and notifies all clients.*/
	void CreateNewPlayer(const int ID, std::vector<std::string> &mesh);
	
private:
	ServerListener* zServerListener;

	MaloW::Array<ClientData*>* zClients;
	MaloW::Array<PlayerActor*>* zPlayers;
	
	NetworkMessageConverter zMessageConverter;

	int	zMaxClients;
	int zPort;
	
	INT64 zStartime;
	float zSecsPerCnt;
	float zDeltaTime;

};
