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
	//If a Host has been Initiated Start the host
	if(zHosting)
		this->zHost->Start();
	
	this->zClient->Start();
}

int Game::InitGameClient(std::string ip, int port)
{
	int code;
	//If a Client Hasn't been created yet Create one
	if(!this->zClient)
		this->zClient = new Client();

	//Connects to a Host With the Ip and port in the parameters
	code = this->zClient->Connect(ip, port);

	return code;
}

int Game::InitGameHost(int port, int nrOfClients)
{
	int code;

	//if a Host hasn't been created yet Create one
	if(!this->zHost)
		this->zHost = new Host();

	//Creates a host to listen on the given port
	code = this->zHost->InitHost(port, nrOfClients);

	this->zHosting = true;
	
	return code;
}

