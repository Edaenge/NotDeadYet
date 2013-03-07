/*
Made by Edänge Simon 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "ServerListener.h"
#include "ClientData.h"
#include "Game.h"
#include "GameModeFFA.h"
#include "GameEvents.h"
#include "GameTimer.h"

class ActorSynchronizer;

#if defined(DEBUG) || defined(_DEBUG)
#include <vld.h>
#define INCLUDE_MODEL_VIEWER
#endif

class Host : public MaloW::Process, public Observed
{
	ServerListener* zServerListener;

	NetworkMessageConverter zMessageConverter;

	unsigned int zMaxClients;
	unsigned int zMinClients;
	int zPort;
	bool zGameStarted;
	bool zRestartRequested;
	float zDeltaTime;
	GameTimer* zGameTimer;
	float zTimeOut;
	float zTimeSinceLastPing;
	float zSendUpdateDelayTimer;
	float zPingMessageInterval;
	std::string zGameMode;
	std::string zMapName;

	std::set<MaloW::ClientChannel*> zClientChannels;
	std::map<MaloW::ClientChannel*, ClientData*> zClients;
	Game* zGame;
	ActorSynchronizer* zSynchronizer;
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

	/*! Notifies all clients, the server is shutting down.*/
	void BroadCastServerShutdown();

	/*! Pings the clients.*/
	//void PingClients();

	/*! Runs the Game Loop.*/
	void UpdateGame();

	/*! */
	void SendToAllClients(const std::string& message, bool important = true);

	/*! Checks if the server is alive.*/
	bool IsAlive() const;

	// Returns Number of active connections
	unsigned int GetNumClients() const;

	// Restart Game
	void Restart( const std::string& gameMode, const std::string& map );

	void SendMessageToClient(const std::string& message);
	// Synchronize Clients
	void SynchronizeAll();
private:
	/*! Handles new incoming connections.*/
	void HandleNewConnection( MaloW::ClientChannel* CC );
	/*! Handles messages from clients.*/
	void HandleReceivedMessage( MaloW::ClientChannel* cc, const std::string &message );
	/*! */
	void HandleClientUpdate(const std::vector<std::string> &msgArray, ClientData* cd);
	/*! Handles if a client disconnects.
		Deletes the client information.
	*/
	void HandleClientDisconnect( MaloW::ClientChannel* channel );
	/*! */
	void HandleLootRequest(const std::vector<std::string> &msgArray, ClientData* cd);
	/*! */
	void HandleUserData(const std::vector<std::string> &msgArray, ClientData* cd);
	/*! Read messages from queue and saves them in*/
	void ReadMessages(); 
	/*! */
	void PingClients();
};