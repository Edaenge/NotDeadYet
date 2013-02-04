#include <Safe.h>
#include "Host.h"

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

	const char* InitHost(const unsigned int& port, const unsigned int& maxPlayers);
	bool StartHost();
	void ShutDownHost();

	bool IsRunning() const;
	int GetNrOfPlayers() const;


private:
	Host* zHost;
};