#include "../../../../Source/MaloWLib/Safe.h"
#include "../EngineFiles/Server/Host.h"

#pragma once

static const enum NETWORK_ERROR_CODE
{
	CONNECTION_SUCCESS, 
	CONNECTION_FAILED,
	CONNECTION_WINSOCK_FAIL,
	CONNECTION_INVALID_SOCKET
};

class ServerEngine
{
public:
	ServerEngine();
	~ServerEngine();

	char* InitHost(const int port, const int maxPlayers);
	bool StartHost();
	void ShutDownHost();

	bool IsRunning() const;
	int GetNrOfPlayers() const;


private:
	Host* zHost;
};