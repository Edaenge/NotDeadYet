/*
Made by Edänge Simon 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Process.h"
#include "ServerListener.h"
#include "ActorHandler.h"
#include "ClientData.h"
#include <World/World.h>

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
	/*! Handles messages from clients. This function will call the following functions:
	HandleCloseConnectionMsg
	HandleKeyPress
	HandleKeyRelease
	CreateNewPlayer
	*/
	void HandleReceivedMessage( const unsigned int &ID, const std::string &message );
	/*! Read messages from queue and saves them in*/
	void ReadMessages(); 

	void HandleDisconnect( MaloW::ClientChannel* channel );
	void Message(MaloW::ClientChannel* cc, std::string msg);
protected:
	virtual void OnEvent(Event* e);

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

};