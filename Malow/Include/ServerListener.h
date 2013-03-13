#pragma once

/*
Made by Edänge Simon 
For project Not Dead Yet at Blekinge tekniska högskola.

Inherited from NetworkServer.h.

This class listens for client connections.
When a client connects, it accepts and notifies it's thread with a 
ClientConnectedEvent.
*/

#include "Process.h"


namespace MaloW
{
	class ServerListener : public Process
	{
	private:
		Process *zObserver;
		SOCKET zListenSocket;
		unsigned int zPort;
	
		bool Accept( SOCKET &newConnection, sockaddr_in &client );

	protected:
		virtual void Life();
	
	public:
		ServerListener( Process *observer, const unsigned int &port ) throw(...);
		virtual ~ServerListener() throw(...);
	};
}