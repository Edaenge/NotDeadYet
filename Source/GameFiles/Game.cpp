#include "Game.h"


Game::Game()
{
	this->zClient = NULL;
	this->zHost = NULL;
	this->zHosting = false;
}

Game::~Game()
{
	SAFE_DELETE(this->zClient);
	SAFE_DELETE(this->zHost);
}

void Game::Run()
{
	if(zHosting)
		this->zHost->Start();
	
	this->zClient->Start();
}

int Game::InitGameClient(std::string ip, int port)
{
	int code;

	if(!this->zClient)
		this->zClient = new Client();

	code = this->zClient->Connect(ip, port);

	return code;
}

int Game::InitGameHost(int port, int nrOfClients)
{
	int code;

	if(!this->zHost)
		this->zHost = new Host();

	code = this->zHost->InitHost(port, nrOfClients);

	this->zHosting = true;
	
	return code;
}

