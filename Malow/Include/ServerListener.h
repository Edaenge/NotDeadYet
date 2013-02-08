// Made by Edänge Simon 
// For project Not Dead Yet at Blekinge tekniska högskola.

#pragma once

#include "ClientConnectedEvent.h"
#include "Process.h"

/*
Inherited from NetworkServer.h.

This class listens for client connections.
When a client connects, it accepts and notifies it's thread with a 
ClientConnectedEvent.
*/
class ServerListener : public MaloW::Process
{
private:
	MaloW::Process *zObserver;
	SOCKET zListenSocket;
	unsigned int zPort;

	bool Accept( SOCKET &newConnection, sockaddr_in &client );
protected:
	virtual void Life();

public:
	ServerListener( MaloW::Process *observer, const unsigned int &port ) throw(...);
	virtual ~ServerListener() throw(...);
	
};