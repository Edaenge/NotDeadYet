/*
Made by Edänge Simon 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "stdafx.h"
#include "Process.h"
#include "Network/ServerListener.h"
#include "Network/NetworkMessageConverter.h"

struct PlayerInfo
{
	PlayerInfo(int id)
	{
		this->zID = id;
		zState = 0;
		zScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		zPos = D3DXVECTOR3(0,0,0);
	}

	D3DXVECTOR3 zPos;
	D3DXVECTOR3 zScale;
	D3DXVECTOR3 zDir;
	D3DXQUATERNION zRot;

	std::string zMeshModel;

	int zState;
	int zID;

};

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
	void SendToAllClients(std::string message);
	/*! Sends to a specific client.*/
	void SendToClient(int clientID, std::string message);
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
	*/
	void HandleRecivedMessages();
	/*! Handles ping messages.*/
	void HandlePingMsg(const int CLIENT_ID);
	/*! Search for a client. Returns -1 if none was found.*/
	int SearchForClient(const int ID) const;
	/*! Search for a player. Returns -1 if none was found.*/
	int SearchForPlayer(const int ID) const;
	/*! Creates a new player and notifies all clients.*/
	void CreateNewPlayer(const int ID, std::string mesh);
	
private:
	ServerListener* zServerListener;

	MaloW::Array<ClientData*>* zClients;
	MaloW::Array<PlayerInfo*>* zPlayers;
	
	NetworkMessageConverter zMessageConverter;

	int	zMaxClients;
	int zPort;
	
	INT64 zStartime;
	float zSecsPerCnt;
	float zDeltaTime;

};
