//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.

#pragma once

#include "Client.h"
#include "SoundHandler.h"


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
		/*! Creates a Client that connects to the IP and Port in the Parameters.
			The function will start the thread if succeeded.*/
		void InitGameClient(const std::string &IP, const unsigned int &port, std::string& errMsg, int& errorCode);
		/*! Initialize the sounds.
			It will read and load all sounds.
		*/
		bool InitSounds(SoundHandler* engine);

		virtual ~Game();

		void Run();

	private:
		Client* zClient;
};