#include "GameFiles/Client.h"
#include "Network/NetworkMessageConverter.h"

using namespace MaloW;

Client::Client()
{
	this->zIP = "";
	this->zPort = 0;
	this->zEng = NULL;
	this->zServerChannel = NULL;
	this->zTimeSinceLastPing = 0.0f;
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
		zTimeSinceLastPing += diff;
		CursorControl cc;
		if (MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			//Check if a New Player has connected
			NewPlayerEvent* npe = dynamic_cast<NewPlayerEvent*>(ev);
			if(npe != NULL)
			{
				this->HandleNewPlayerEvent(npe);
			}
			//Check if a Player has updated
			PlayerUpdateEvent* pue = dynamic_cast<PlayerUpdateEvent*>(ev);
			if(pue != NULL)
			{
				this->HandlePlayerUpdateEvent(pue);
			}
			//Check if Client has recieved a Ping
			NetworkPacket* np = dynamic_cast<NetworkPacket*>(ev);
			if (np != NULL)
			{
				this->HandlePingEvent(np);
			}
			delete ev;
			ev = NULL;
		}
		this->HandleKeyboardInput();
	}
}
bool Client::IsAlive()
{
	return stayAlive;
}
void Client::HandleNewPlayerEvent(NewPlayerEvent* ev)
{
	Player* newPlayer = new Player();
	D3DXVECTOR3 position = ev->GetPlayerPosition();
	D3DXVECTOR3 scale = ev->GetPlayerScale();
	D3DXQUATERNION rotation = ev->GetPlayerRotation();
	string filename = ev->GetFilename();
	int clientID = ev->GetClientID();

	StaticMesh* playerMesh = this->zEng->CreateStaticMesh(filename, position);
	playerMesh->SetQuaternion(rotation);
	playerMesh->Scale(scale);

	newPlayer->AddStaticMesh(playerMesh);
	newPlayer->SetClientID(clientID);
	bool found = false;
	for (int i = 0; i < this->zPlayers.size(); i++)
	{
		if (this->zPlayers.get(i)->GetClientID() == clientID)
		{
			found = true;
		}
	}
	this->zPlayers.add(newPlayer);
}
void Client::HandlePlayerUpdateEvent(PlayerUpdateEvent* ev)
{
	int clientID = ev->GetClientID();
	for (int i = 0; i < this->zPlayers.size(); i++)
	{
		if (clientID == this->zPlayers.get(i)->GetClientID())
		{
			if(clientID == 0)
			{
				//Todo something....
			}
			StaticMesh* playerMesh = this->zPlayers.get(i)->GetPlayerMesh();
			//Check if Value has Been added
			if (ev->HasNewRotation())
			{
				playerMesh->SetQuaternion(ev->GetPlayerRotation());
			}
			if (ev->HasNewPosition())
			{
				playerMesh->SetPosition(ev->GetPlayerPosition());
			}
			if (ev->HasNewFile())
			{
				this->zPlayers.get(i)->GetPlayerMesh()->LoadFromFile(ev->GetFilename());
			}
		}
	}
}
void Client::HandleKeyboardInput()
{
	if (this->zEng->GetKeyListener()->IsPressed('W'))
	{
		//this->zEng->GetCamera()->moveForward(diff);
		this->zServerChannel->sendData("BP W");
	}
	if(this->zEng->GetKeyListener()->IsPressed('A'))
	{
		//this->zEng->GetCamera()->moveLeft(diff);
		this->zServerChannel->sendData("BP A");
	}
	if(this->zEng->GetKeyListener()->IsPressed('S'))	
	{
		//this->zEng->GetCamera()->moveBackward(diff);
		this->zServerChannel->sendData("BP S");
	}
	if(this->zEng->GetKeyListener()->IsPressed('D'))	
	{
		//this->zEng->GetCamera()->moveRight(diff);
		this->zServerChannel->sendData("BP D");
	}
	if (this->zEng->GetKeyListener()->IsPressed(VK_ESCAPE))
	{
		this->zServerChannel->sendData("CC");

		this->Close();
	}
}
void Client::Ping()
{
	this->zServerChannel->sendData(PING);
}
void Client::HandlePingEvent(NetworkPacket* ev)
{
	if (zTimeSinceLastPing > 10.0f)
	{
		//todo close serverClient etc
		this->Close();
	}
	else
	{
		this->zTimeSinceLastPing = 0;
		this->Ping();
	}
	
}
