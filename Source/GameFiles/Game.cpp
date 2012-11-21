#include "Game.h"


Game::Game()
{
	this->zClient = NULL;
	this->zHost = NULL;
}

Game::~Game()
{
	SAFE_DELETE(zClient);
	SAFE_DELETE(zHost);
}

void Game::Run()
{
	
}

int Game::InitGameClient(std::string ip, int port)
{
	int code;

	if(!zClient)
		zClient = new Client();

	code = zClient->Connect(ip, port);

	return code;
}

int Game::InitGameHost(int port, int nrOfClients)
{
	int code;

	if(!zHost)
		zHost = new Host();

	code = zHost->InitHost(port)

	return code;
}
