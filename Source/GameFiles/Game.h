//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.

#pragma once

#include "ClientSide\Client.h"


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
		/*! Creates a Client that connects to the Ip and Port in the Parameters.
			The function will start the thread if succeeded.*/
		int InitGameClient(std::string ip, int port);
		/*! Creates a Host that listens on the given Port
			The function will start the thread if succeeded.*/
		int InitGameHost(int port, int nrOfClients);
		virtual ~Game();

		void Run();

	private:
		Client* zClient;
};