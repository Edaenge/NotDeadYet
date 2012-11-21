#include "Client.h"

Client::Client()
{
	this->zIP = "";
	this->zPort = 0;
	this->zEng = NULL;
	this->zServerChannel = NULL;
}
int Client::Connect(std::string ip, int port)
{
	int code = 0;

	this->zIP = ip;
	this->zPort = port;
	this->zEng = GetGraphicsEngine();
	this->zServerChannel = new MaloW::ServerChannel();

	code = zServerChannel->InitConnection(ip, port);

	return code;
}
Client::~Client()
{
	
}
void Client::Life()
{
	this->zServerChannel->Start();

	while(this->stayAlive)
	{
		this->zEng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6f));
		this->zEng->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));

		this->zEng->CreateTerrain(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(100, 1, 100), "Media/TerrainTexture.png", "Media/TerrainHeightmap.raw");

		StaticMesh* bth = this->zEng->CreateStaticMesh("Media/scale.obj", D3DXVECTOR3(5, -6, 15));

		bth->Scale(0.1f);
		Light* testLight = this->zEng->CreateLight(D3DXVECTOR3(15, 30, 15));

		//SoundEngine* seng = eng->GetSoundEngine();
		//seng->SetMasterVolume(0.5f);

		this->zEng->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);
		
		float diff = this->zEng->Update();
		CursorControl cc;

		if (this->zEng->GetKeyListener()->IsPressed('W'))
		{
			this->zServerChannel->sendData("");
		}


		if (this->zEng->GetKeyListener()->IsPressed(VK_ESCAPE) || 
			!this->zEng->isRunning())
		{
			this->zServerChannel->sendData("");
			this->Close();
		}

	}
}
