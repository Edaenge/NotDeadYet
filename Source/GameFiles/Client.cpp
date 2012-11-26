#include "GameFiles/Client.h"
#include "Network/NewPlayerEvent.h"

using namespace MaloW;

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
	this->zServerChannel = new ServerChannel();
	//Tries to Connect to a server with the specified Ip and Port
	code = this->zServerChannel->InitConnection(ip, port);
	
	return code;
}
Client::~Client()
{
	while (this->zPlayers.size() > 0)
	{
		this->zPlayers.remove(0);
	}
	SAFE_DELETE(this->zServerChannel);
}
void Client::Life()
{
	this->zServerChannel->setNotifier(this);

	this->zServerChannel->Start();

	this->zEng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6f));
	this->zEng->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));

	this->zEng->CreateTerrain(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(100, 1, 100), "Media/TerrainTexture.png", "Media/TerrainHeightmap.raw");
	
	StaticMesh* scaleHuman = this->zEng->CreateStaticMesh("Media/scale.obj", D3DXVECTOR3(5, -6, 15));
	scaleHuman->Scale(0.1f);
	this->zEng->GetCamera()->FollowMesh(scaleHuman);

	Light* testLight = this->zEng->CreateLight(D3DXVECTOR3(15, 30, 15));
	testLight->SetIntensity(50);
	//SoundEngine* seng = eng->GetSoundEngine();
	//seng->SetMasterVolume(0.5f);

	this->zEng->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);

	Player* player = new Player();
	player->AddStaticMesh(scaleHuman);
	this->zPlayers.add(player);

	//todo Skicka meddelanden till server och få reda på self id

	while(this->stayAlive)
	{
		float diff = this->zEng->Update();
		CursorControl cc;
		
		if (MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			NewPlayerEvent* npe = dynamic_cast<NewPlayerEvent*>(ev);
			if(npe != NULL)
			{
				Player* newPlayer = new Player();
				D3DXVECTOR3 position = npe->GetPlayerPosition();
				D3DXVECTOR3 scale = npe->GetPlayerScale();
				D3DXQUATERNION rotation = npe->GetPlayerRotation();
				string filename = npe->GetFilename();

				StaticMesh* playerMesh = this->zEng->CreateStaticMesh(filename, position);
				playerMesh->SetQuaternion(rotation);
				playerMesh->Scale(scale);

				newPlayer->AddStaticMesh(playerMesh);
				this->zPlayers.add(newPlayer);
			}
			PlayerUpdateEvent* pue = dynamic_cast<PlayerUpdateEvent*>(ev);
			if(pue != NULL)
			{
				int clientID = pue->GetClientID();
				for (int i = 0; i < this->zPlayers.size(); i++)
				{
					if (clientID == this->zPlayers.get(i)->GetClientID())
					{
						if (clientID == 0)
						{
							
						}
						StaticMesh* playerMesh = this->zPlayers.get(i)->GetPlayerMesh();
						playerMesh->SetQuaternion(pue->GetPlayerRotation());
						playerMesh->SetPosition(pue->GetPlayerPosition());
						bool newMesh = pue->HasNewFile();
						if (newMesh)
						{
							std::string filename = pue->GetFilename();
							this->zPlayers.get(i)->GetPlayerMesh()->LoadFromFile(filename);
						}
					}
					
				}
			}
		}
		if (this->zEng->GetKeyListener()->IsPressed('W'))
		{
			this->zEng->GetCamera()->moveForward(diff);
			this->zServerChannel->sendData("");
		}


		if (this->zEng->GetKeyListener()->IsPressed(VK_ESCAPE))
		{
			this->zServerChannel->sendData("CC");
			
			this->Close();
		}
	}
}
