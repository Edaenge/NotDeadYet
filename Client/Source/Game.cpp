#include "Game.h"
#include "Graphics.h"
#include <fstream>
#include <Safe.h>

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

bool Game::InitGameClient(const std::string &IP, const unsigned int &port)
{
	//If a Client Hasn't been created yet Create one
	if(this->zClient)
		SAFE_DELETE(this->zClient);

	//Connects to a Host With the IP Address and port in the parameters
	// TODO: RAII
	this->zClient = new Client();
	iImage* blackImage = GetGraphics()->CreateImage(Vector2(0,0), Vector2( (float)GetGraphics()->GetEngineParameters().WindowWidth, 
		(float)GetGraphics()->GetEngineParameters().WindowHeight), "Media/LoadingScreen/FadeTexture.png" );
	this->zClient->SetBlackImage(blackImage);
	blackImage = NULL;
	bool result = this->zClient->Connect(IP, port);

	if (result)
		this->zClient->Start();

	return result;
}