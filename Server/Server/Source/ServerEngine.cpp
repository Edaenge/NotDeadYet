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

int ServerEngine::GetNrOfPlayers() const
{
	return 0;//this->zHost->GetNrOfPlayers();
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

const char* ServerEngine::InitHost( const unsigned int& port, const unsigned int& maxPlayers )
{
	zHost = new Host();
	return zHost->InitHost(port, maxPlayers);
}

