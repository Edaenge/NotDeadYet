#include "../EngineFiles/ServerEngine.h"


ServerEngine::ServerEngine()
{
	zHost =  NULL;
}

ServerEngine::~ServerEngine()
{
	SAFE_DELETE(zHost);
}

char* ServerEngine::InitHost( const int port, const int maxPlayers )
{
	char* ret = NULL;
	int code;

	if(zHost)
	{
		SAFE_DELETE(this->zHost);
		this->zHost = new Host();
	}
	else
		this->zHost = new Host();

	if(maxPlayers <= 0)
		code = zHost->InitHost(port,1);

	code = zHost->InitHost(port,maxPlayers);

	std::string msg = "";
	switch (code)
	{
	case CONNECTION_FAILED:
		msg += "Failed to bind socket.";
		break;
	case CONNECTION_WINSOCK_FAIL:
		msg += "Failed to init Winsock library.";
		break;
	case CONNECTION_INVALID_SOCKET:
		msg += "Invalid socket, failed to create socket.";
		break;
		default:
			ret = NULL;
		break;
	}

	//Success
	if(msg == "")
	{
		return ret;
	}

	//Fail
	SAFE_DELETE(this->zHost);

	ret = new char[50];
	strcpy(ret, msg.c_str());
		
	return ret;
}

bool ServerEngine::IsRunning() const
{
	if(this->zHost)
		return this->zHost->IsAlive();

	return false;
}

int ServerEngine::GetNrOfPlayers() const
{
	return this->zHost->GetNrOfPlayers();
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

