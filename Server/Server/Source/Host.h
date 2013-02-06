/*
Made by Edänge Simon 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "ServerListener.h"
#include "ActorHandler.h"
#include "ClientData.h"
#include "Game.h"
#include "GameModeFFA.h"
#include "GameEvents.h"
#include "ActorSynchronizer.h"

class World;

#if defined(DEBUG) || defined(_DEBUG)
#include <vld.h>
#define INCLUDE_MODEL_VIEWER
#endif

class Host : public MaloW::Process, public Observed
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	const char* InitHost(const unsigned int &port, const unsigned int &maxClients, const std::string& gameMode, const std::string& mapName);
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

	/*! Notifies all clients, the server is shutting down.*/
	void BroadCastServerShutdown();
	/*! Pings the clients.*/
	//void PingClients();
	/*! Updates the server clock.*/
	float Update();

	/*! Checks if the server is alive.*/
	bool IsAlive() const;

private:
	/*! Handles new incoming connections.*/
	void HandleNewConnection( MaloW::ClientChannel* CC );
	/*! Handles messages from clients.*/
	void HandleReceivedMessage( MaloW::ClientChannel* cc, const std::string &message );
	/*! */
	void HandleClientUpdate(const std::vector<std::string> msgArray, ClientData* cd);
	/*! */
	void HandleDisconnect( MaloW::ClientChannel* channel );
	/*! */
	void HandleLootRequest(const std::vector<std::string> &msgArray, ClientData* cd);
	/*! */
	void HandleUserData(const std::vector<std::string> &msgArray, ClientData* cd);
	/*! Read messages from queue and saves them in*/
	void ReadMessages(); 

	void Message(MaloW::ClientChannel* cc, std::string msg);

private:
	ServerListener* zServerListener;
	
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

	std::map<MaloW::ClientChannel*, ClientData*> _clients;
	Game* zGame;
};