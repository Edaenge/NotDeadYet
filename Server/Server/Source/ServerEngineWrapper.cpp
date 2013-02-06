//File: ServerEngineWrapper.cpp

#include "ServerEngineWrapper.h"
#include "ServerEngine.h"

ServerEngineWrapper::ServerEngineWrapper()
{
	zServerEngine = new ServerEngine();
}

ServerEngineWrapper::~ServerEngineWrapper()
{
	SAFE_DELETE(zServerEngine);
}

const char* ServerEngineWrapper::Init(const unsigned int &port, const unsigned int &maxPlayers, const char* gameMode, const char* mapName) const
{
	return zServerEngine->InitHost(port, maxPlayers, gameMode, mapName);
}

void ServerEngineWrapper::ShutdownHost() const
{
	this->zServerEngine->ShutDownHost();
}

bool ServerEngineWrapper::StartHost() const
{
	return this->zServerEngine->StartHost();
}

int ServerEngineWrapper::GetNrOfPlayers() const
{
	return zServerEngine->GetNrOfPlayers();
}

char* ServerEngineWrapper::ProcessText(char* msg)
{
	//return zServerEngine->ProcessText(msg);
	return NULL;
}

bool ServerEngineWrapper::IsRunning() const
{
	return zServerEngine->IsRunning();
}