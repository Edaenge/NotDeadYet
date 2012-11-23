//Made by Edänge Simon 
//for project desperation* at Blekinge tekniska högskola.

#pragma once

#include "Client.h"
#include "Host.h"
#include "stdafx.h"

enum NETWORK_ERROR_CODE
{
	CONNECTION_SUCCESS, 
	CONNECTION_FAILED,
	CONNECTION_WINSOCK_FAIL,
	CONNECTION_INVALID_SOCKET

};

class Game
{
	public:
		Game();
		int InitGameClient(std::string ip, int port);
		int InitGameHost(int port, int nrOfClients);
		virtual ~Game();

		void Run();

	private:
		Client* zClient;
		Host*	zHost; 
		bool	zHosting;
};