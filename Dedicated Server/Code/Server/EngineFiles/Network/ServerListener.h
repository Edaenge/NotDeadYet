//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.

#pragma once

#include "NetworkServer.h"
#include "ClientConnectedEvent.h"

/*
Inherited from NetworkServer.h.

This class listens for client connections.
When a client connects, it accepts and notifies it's thread with a 
ClientConnectedEvent.
*/
class ServerListener : public MaloW::NetworkServer
{
public:
	ServerListener();
	virtual ~ServerListener();

	void ClientConnected(MaloW::ClientChannel* cc);

	/*! Inits the server listener. This one is manually called before starting the thread. */
	int InitListener(int port);

private:

};