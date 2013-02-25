#include "ServerEngine.h"


ServerEngine::ServerEngine()
{
	zHost =  NULL;
}

ServerEngine::~ServerEngine()
{
	SAFE_DELETE(zHost);
}

bool ServerEngine::IsRunning() const
{
	if(this->zHost)
		return this->zHost->IsAlive();

	return false;
}

unsigned int ServerEngine::GetNumClients() const
{
	return zHost->GetNumClients();
}

bool ServerEngine::StartHost()
{
	if(!this->zHost)
		return false;

	this->zHost->Start();

	return true;
}

void ServerEngine::ShutDownHost()
{
	SAFE_DELETE(this->zHost);
}

const char* ServerEngine::InitHost( const unsigned int& port, const unsigned int& maxPlayers, const std::string& gameMode, const std::string& mapName )
{
	zHost = new Host();
	return zHost->InitHost(port, maxPlayers, gameMode, mapName);
}

void ServerEngine::SendMessageToClient( const std::string& message )
{
	this->zHost->SendMessageToClient(message);
}