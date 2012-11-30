#include "Client.h"
#include "Graphics.h"
#include "Safe.h"
#include "NetworkPacket.h"

using namespace MaloW;

static const float TIMEOUT_VALUE; 
Client::Client()
{
	this->zIP = "";
	this->zPort = 0;
	this->zEng = NULL;
	this->zServerChannel = NULL;
	this->zTimeSinceLastPing = 0.0f;
	this->msgHandler = NetworkMessageConverter();

	zMeshID = "Media/scale.obj";
}


int Client::Connect(const std::string& ip, const int port)
{
	int code = 0;

	this->zIP = ip;
	this->zPort = port;
	this->zEng = GetGraphics();
	this->zServerChannel = new ServerChannel();
	//Tries to Connect to a server with the specified Ip and Port
	code = this->zServerChannel->InitConnection(ip, port);
	
	return code;
}


Client::~Client()
{
	for( auto x = zPlayers.begin(); x < zPlayers.end(); ++x )
	{
		delete *x;
	}

	SAFE_DELETE(this->zServerChannel);
}


void Client::initClient()
{
	this->zServerChannel->setNotifier(this);

	this->zServerChannel->Start();

	this->zEng->GetCamera()->SetPosition( Vector3(0, 15, -15.6f) );
	this->zEng->GetCamera()->LookAt( Vector3(30, 10, 10) );

	this->zEng->CreateTerrain(Vector3(0, 0, 0), Vector3(100, 1, 100), "Media/TerrainTexture.png", "Media/TerrainHeightmap.raw");

	//StaticMesh* scaleHuman = this->zEng->CreateStaticMesh("Media/scale.obj", D3DXVECTOR3(5, -6, 15));
	//scaleHuman->Scale(0.1f);
	//this->zEng->GetCamera()->FollowMesh(scaleHuman);

	iLight* testLight = this->zEng->CreateLight(Vector3(15, 30, 15));
	testLight->SetIntensity(50);
	//SoundEngine* seng = eng->GetSoundEngine();
	//seng->SetMasterVolume(0.5f);

	//this->zEng->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);
}


void Client::Life()
{
	while(this->stayAlive)
	{
		float diff = this->zEng->Update();
		zTimeSinceLastPing += diff;

		if (MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			//Check if Client has recieved a Message
			NetworkPacket* np = dynamic_cast<NetworkPacket*>(ev);
			if (np != NULL)
			{
				this->HandleNetworkMessage(np->getMessage());
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


void Client::HandleKeyboardInput()
{
	if (this->zEng->GetKeyListener()->IsPressed('W'))
	{
		//this->zEng->GetCamera()->moveForward(diff);
		this->zServerChannel->sendData(KEY_PRESS+"W");
	}
	if(this->zEng->GetKeyListener()->IsPressed('A'))
	{
		//this->zEng->GetCamera()->moveLeft(diff);
		this->zServerChannel->sendData(KEY_PRESS+"A");
	}
	if(this->zEng->GetKeyListener()->IsPressed('S'))	
	{
		//this->zEng->GetCamera()->moveBackward(diff);
		this->zServerChannel->sendData(KEY_PRESS+"S");
	}
	if(this->zEng->GetKeyListener()->IsPressed('D'))	
	{
		//this->zEng->GetCamera()->moveRight(diff);
		this->zServerChannel->sendData(KEY_PRESS+"D");
	}
	if (this->zEng->GetKeyListener()->IsPressed(VK_ESCAPE))
	{
		this->zServerChannel->sendData(CONNECTION_CLOSED + MaloW::convertNrToString((float)this->zID));

		this->Close();
	}
}


void Client::Ping()
{
	if (zTimeSinceLastPing <= TIMEOUT_VALUE)
	{
		this->zTimeSinceLastPing = 0.0f;
		this->zServerChannel->sendData(PING);
	}
	else
	{
		this->CloseConnection("Connection timeout");
	}
}


void Client::HandleNetworkMessage(std::string msg)
{
	std::vector<std::string> msgArray;
	msgArray = this->msgHandler.SplitMessage(msg);
	char key[1024];
	if(msgArray.size() > 0)
	{
		sscanf_s(msgArray[0].c_str(), "%s ", key);

		//Checks what type of message was sent
		if(strcmp(key, PING.c_str()) == 0)
		{
			this->Ping();
		}
		else if(strcmp(key, PLAYER_UPDATE.c_str()) == 0)
		{
			this->HandlePlayerUpdate(msgArray);
		}
		else if(strcmp(key, NEW_PLAYER.c_str()) == 0)
		{
			this->HandleNewPlayer(msgArray);
		}
		else if(strcmp(key, REMOVE_PLAYER.c_str()) == 0)
		{
			this->HandleRemovePlayer(msgArray);
		}
		else if(strcmp(key, SELF_ID.c_str()) == 0)
		{
			this->zID = this->msgHandler.ConvertStringToInt(SELF_ID, msgArray[0]);
			
			std::string serverMessage = "";
			serverMessage = this->msgHandler.Convert(MESSAGE_TYPE_USER_DATA, this->zMeshID);
			this->zServerChannel->sendData(serverMessage);
		}
		else if(strcmp(key, SERVER_FULL.c_str()) == 0)
		{
			this->CloseConnection("Server is full");
		}
		else if(strcmp(key, KICKED.c_str()) == 0)
		{
			this->CloseConnection("You got kicked");
		}
		else if(strcmp(key, SERVER_SHUTDOWN.c_str()) == 0)
		{
			this->CloseConnection("Server Shutdown");
		}
	}
}


void Client::CloseConnection(const std::string reason)
{
	//To do printa till client vilket reason som gavs
	this->zServerChannel->Close();
	this->Close();
}


void Client::HandleNewPlayer(const std::vector<std::string>& msgArray)
{
	char key[512];

	Vector3 position;
	Vector3 scale;
	Vector4 rotation;
	std::string filename;
	int clientID = -1;
	int state = 0;

	Player* newPlayer = new Player();
	for(unsigned int i = 0; i < msgArray.size(); i++)
	{
		sscanf_s(msgArray[i].c_str(), "%s ", key);

		if(strcmp(key, NEW_PLAYER.c_str()) == 0)
		{
			clientID = this->msgHandler.ConvertStringToInt(NEW_PLAYER, msgArray[i]);
		}
		else if(strcmp(key, POSITION.c_str()) == 0)
		{
			position = this->msgHandler.ConvertStringToVector(POSITION, msgArray[i]);
		}
		else if(strcmp(key, ROTATION.c_str()) == 0)
		{
			rotation = this->msgHandler.ConvertStringToQuaternion(ROTATION, msgArray[i]);
		}
		else if(strcmp(key, STATE.c_str()) == 0)
		{
			state = this->msgHandler.ConvertStringToInt(STATE, msgArray[i]);
		}
		else if(strcmp(key, SCALE.c_str()) == 0)
		{
			scale = this->msgHandler.ConvertStringToVector(SCALE, msgArray[i]);
		}
		else if(strcmp(key, MESH_MODEL.c_str()) == 0)
		{
			filename = this->msgHandler.ConvertStringToSubstring(MESH_MODEL, msgArray[i]);
		}
	}
	if (clientID != -1)
	{
		bool found = false;
		for (unsigned int i = 0; i < this->zPlayers.size(); i++)
		{
			int id = this->zPlayers[i]->GetClientID();
			if(clientID == id)
			{
				found = true;
				MaloW::Debug("Duplicate ID from server");
			}
		}
		if (!found)
		{
			//Creates a StaticMesh from the given Filename
			iMesh* playerMesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
			playerMesh->SetQuaternion(rotation);
			playerMesh->Scale(scale);

			if (clientID == this->zID)
			{
				this->zEng->GetCamera()->FollowMesh(playerMesh);
			}
			//Create player data
			newPlayer->AddStaticMesh(playerMesh);
			newPlayer->SetClientID(clientID);
			newPlayer->SetPlayerState(state);

			this->zPlayers.push_back(newPlayer);
			newPlayer = NULL;
		}
	}
}


void Client::HandlePlayerUpdate(const std::vector<std::string>& msgArray)
{
	char key[512];

	int clientID = -1;
	int ClientPosition = -1;
	bool clientFound = false;
	for( unsigned int i=0; i <msgArray.size(); ++i )
	{
		sscanf_s(msgArray[i].c_str(), "%s ", key);

		if(strcmp(key, PLAYER_UPDATE.c_str()) == 0)
		{
			clientID = this->msgHandler.ConvertStringToInt(PLAYER_UPDATE, msgArray[i]);

			ClientPosition = i;
			clientFound = true;
			break;
		}
		i++;
	}

	if(clientFound)
	{
		Player* playerPointer = this->zPlayers[ClientPosition];
		for(unsigned int i = 0; i < msgArray.size(); i++)
		{
			sscanf_s(msgArray[i].c_str(), "%s ", key);

			if(strcmp(key, POSITION.c_str()) == 0)
			{
				Vector3 position = this->msgHandler.ConvertStringToVector(POSITION, msgArray[i]);
				playerPointer->SetPlayerPosition(position);
			}
			else if(strcmp(key, ROTATION.c_str()) == 0)
			{
				Vector4 rotation = this->msgHandler.ConvertStringToQuaternion(ROTATION, msgArray[i]);
				playerPointer->SetPlayerRotation(rotation);
			}
			else if(strcmp(key, STATE.c_str()) == 0)
			{
				int playerState = this->msgHandler.ConvertStringToInt(STATE, msgArray[i]);
				playerPointer->SetPlayerState(playerState);
			}
			else if(strcmp(key, MESH_MODEL.c_str()) == 0)
			{
				std::string filename = this->msgHandler.ConvertStringToSubstring(MESH_MODEL, msgArray[i]);
				
				//Create a new Mesh with the current values
				iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), playerPointer->GetPlayerPosition());
				float scale = playerPointer->GetPlayerScale().x;
				Vector3 quat = playerPointer->GetPlayerRotation();
				
				mesh->Scale(scale);
				mesh->SetQuaternion(Vector4(quat.x,quat.y,quat.z,0.0f));
				playerPointer->AddStaticMesh(mesh);
			}
		}
		//Copy over new Values to the vector
		this->zPlayers[ClientPosition] = playerPointer;
		playerPointer = NULL;
	}
}


void Client::HandleRemovePlayer(const std::vector<std::string>& msgArray)
{
	char key[512];
	int clientID = -1;
	for(unsigned int i = 0; i < msgArray.size(); i++)
	{
		sscanf_s(msgArray[i].c_str(), "%s ", key);

		if(strcmp(key, REMOVE_PLAYER.c_str()) == 0)
		{
			clientID = this->msgHandler.ConvertStringToInt(REMOVE_PLAYER, msgArray[i]);
		}
	}
	//Check if client was found in the array
	if(clientID != -1)
	{
		int temporaryClientID = -1;
		int pos = -1;
		bool isClient = false;
		for (unsigned int i = 0; i < this->zPlayers.size(); i++)
		{
			temporaryClientID = this->zPlayers[i]->GetClientID();
			if (temporaryClientID == clientID)
			{
				//check if the player being removed is the client
				if (temporaryClientID == this->zID)
				{
					isClient = true;
				}
				pos = i;
			}
		}
		//Check if the position is valid
		if (pos != -1)
		{
			if(isClient)
			{
				this->CloseConnection("Unknown reason");
			}

			this->zPlayers.erase( zPlayers.begin() + pos);
		}
	}
}
