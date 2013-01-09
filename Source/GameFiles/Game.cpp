#include "Game.h"
#include "Safe.h"
#include "Graphics.h"

Game::Game()
{
	this->zClient = NULL;
}

Game::~Game()
{
	SAFE_DELETE(this->zClient);
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