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
			if(!bCurrentCursorVisible)
				eng->GetKeyListener()->SetMousePosition(Vector2((float)eng->GetEngineParameters().WindowWidth/2, (float)eng->GetEngineParameters().WindowHeight/2));
			eng->GetCamera()->SetUpdateCamera(!bCurrentCursorVisible);
			eng->GetKeyListener()->SetCursorVisibility(bCurrentCursorVisible);
		}
		
		Sleep(5);
	}
}

bool Game::InitGameClient(const std::string &IP, const unsigned int &port, const std::string &playerModel)
{
	//If a Client Hasn't been created yet Create one
	if(this->zClient)
		SAFE_DELETE(this->zClient);

	//Connects to a Host With the IP Address and port in the parameters
	// TODO: RAII
	this->zClient = new Client(playerModel);
	//iImage* blackImage = GetGraphics()->CreateImage(Vector2(0,0), Vector2( (float)GetGraphics()->GetEngineParameters().WindowWidth, 
	//	(float)GetGraphics()->GetEngineParameters().WindowHeight), "Media/LoadingScreen/FadeTexture.png" );
	//blackImage->SetStrata(50.0f);
	//float windowHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;

	/*iText* connectingText =  GetGraphics()->CreateText("Connecting...", Vector2((50.0f / 1024.0f) * dx, (700.0f / 768.0f) * GetGraphics()->GetEngineParameters().WindowHeight), 2.0f, "Media/Fonts/new");*/
	//this->zClient->SetBlackImage(blackImage);
	//blackImage = NULL;

	bool result = this->zClient->Connect(IP, port);

	/*GetGraphics()->DeleteText(connectingText);
	connectingText = NULL;*/
	if (result)
	{
		GetGraphics()->ShowLoadingScreen("media/loadingScreen/loadingscreenbg.png", "media/loadingscreen/loadingscreenpb.png", 1.0f, 0.2f);
		this->zClient->Start();
	}

	return result;
}