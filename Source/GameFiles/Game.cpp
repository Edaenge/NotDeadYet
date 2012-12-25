#include "Game.h"
#include "Safe.h"
#include "Graphics.h"

Game::Game()
{
	this->zClient = NULL;
	this->zHost = NULL;
}

Game::~Game()
{
	SAFE_DELETE(this->zClient);
	SAFE_DELETE(this->zHost);
}

void Game::Run()
{
	GraphicsEngine* eng = GetGraphics();
	bool bLastCursorVisible = false;
	bool bCurrentCursorVisible = false;
	//Waits for client to exit.
	while(this->zClient->IsAlive())
	{
		eng->Update();
		bCurrentCursorVisible = this->zClient->GetCursorVisibility();
		if (bLastCursorVisible != bCurrentCursorVisible)
		{
			bLastCursorVisible = bCurrentCursorVisible;
			eng->GetCamera()->SetUpdateCamera(!bCurrentCursorVisible);
			eng->GetKeyListener()->SetCursorVisibility(bCurrentCursorVisible);
		}
		
		Sleep(5);
	}

}

int Game::InitGameClient(std::string ip, int port)
{
	int code;

	//If a Client Hasn't been created yet Create one
	if(!this->zClient)
		this->zClient = new Client();
	else
	{
		SAFE_DELETE(this->zClient);
		this->zClient = new Client();
	}

	//Connects to a Host With the Ip and port in the parameters
	code = this->zClient->Connect(ip, port);

	if(code == CONNECTION_SUCCESS)
		this->zClient->Start();
		

	return code;
}

int Game::InitGameHost(int port, int nrOfClients)
{
	int code;

	//if a Host hasn't been created yet Create one
	if(!this->zHost)
		this->zHost = new Host();
	else
	{
		SAFE_DELETE(this->zHost);
		this->zHost = new Host();
	}

	//Creates a host to listen on the given port
	code = this->zHost->InitHost(port, nrOfClients);

	if(code == CONNECTION_SUCCESS)
		this->zHost->Start();
	return code;
}