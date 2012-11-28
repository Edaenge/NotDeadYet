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
	}

	D3DXVECTOR3 zPos;
	D3DXQUATERNION zRot;

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
	*/
	bool KickClient(int ID, bool sendAMessage = false, std::string reason = "");
	/*Checks if the server is alive.*/
	bool IsAlive() const;

private:
	/*Handles new incoming connections.*/
	void HandleNewConnections();
	/*Handles messages from clients.*/
	void HandleRecivedMessages();
	/*Search for a client. Returns -1 if none was found.*/
	int SearchForClient(int ID);
	
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
