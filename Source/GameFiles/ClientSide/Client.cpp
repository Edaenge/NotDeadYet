#include "GameFiles/ClientSide/Client.h"
#include "Graphics.h"
#include "Safe.h"
#include "Network/NetworkPacket.h"

using namespace MaloW;

//Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f; 
static const float UPDATE_DELAY = 0.0333f;

enum MSG_TYPE
{
	PLAYER,
	STATIC_OBJECT,
	DYNAMIC_OBJECT,
	ANIMAL
};

Client::Client()
{
	this->zID = 0;
	this->zIP = "";
	this->zPort = 0;
	this->zEng = NULL;
	this->zRunning = true;
	this->zCreated = false;
	this->zFrameTime = 0.0f;
	this->zWaitTimer = 0.0f;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	this->zServerChannel = NULL;
	this->zTimeSinceLastPing = 0.0f;
	this->zMsgHandler = NetworkMessageConverter();

	zMeshID = "Media/scale.obj";

	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);
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
	this->Close();
	this->WaitUntillDone();
	for(auto x = zPlayers.begin(); x < zPlayers.end(); x++)
	{
		SAFE_DELETE(*x);
	}

	for(auto x = zAnimals.begin(); x < zAnimals.end(); x++)
	{
		SAFE_DELETE(*x);
	}

	for(auto x = zStaticObjects.begin(); x < zStaticObjects.end(); x++)
	{
		SAFE_DELETE(*x);
	}

	for(auto x = zDynamicObjects.begin(); x < zDynamicObjects.end(); x++)
	{
		SAFE_DELETE(*x);
	}

	SAFE_DELETE(this->zServerChannel);
}

float Client::Update()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->zStartime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zStartime = currentTime;

	this->zFrameTime += this->zDeltaTime;

	return this->zDeltaTime;
}

void Client::InitGraphics()
{
	this->zEng->CreateSkyBox("Media/skymap.dds");
	this->zEng->GetCamera()->SetPosition( Vector3(1, 4, -1) );
	this->zEng->GetCamera()->LookAt( Vector3(0, 0, 0) );

	iMesh* objectMesh = this->zEng->CreateStaticMesh("Media/Fern_02_v01.obj", Vector3(0, 0, 0));

	StaticObject* object = new StaticObject();
	object->AddStaticMesh(objectMesh);
	this->zStaticObjects.push_back(object);

	this->zEng->StartRendering();

	//this->zEng->CreateTerrain(Vector3(0, 0, 0), Vector3(100, 1, 100), "Media/TerrainTexture.png", "Media/TerrainHeightmap.raw");

	//StaticMesh* scaleHuman = this->zEng->CreateStaticMesh("Media/scale.obj", D3DXVECTOR3(5, -6, 15));
	//scaleHuman->Scale(0.1f);
	//this->zEng->GetCamera()->FollowMesh(scaleHuman);

	//iLight* testLight = this->zEng->CreateLight(Vector3(15, 30, 15));
	//testLight->SetIntensity(50);
	//SoundEngine* seng = eng->GetSoundEngine();
	//seng->SetMasterVolume(0.5f);

	//this->zEng->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);
}

void Client::Life()
{
	this->Update();
	this->zServerChannel->setNotifier(this);

	this->zServerChannel->Start();

	Sleep(1000);
	this->InitGraphics();

	while(this->zEng->IsRunning() && this->stayAlive)
	{
		this->Update();

		this->zWaitTimer += this->zDeltaTime;
		this->zTimeSinceLastPing += this->zDeltaTime;

		this->HandleKeyboardInput();
		if (this->stayAlive)
		{
			this->UpdateCameraPos();

			this->UpdateWorldObjects();

			if (MaloW::ProcessEvent* ev = this->PeekEvent())
			{
				//Check if Client has received a Message
				NetworkPacket* np = dynamic_cast<NetworkPacket*>(ev);
				if (np != NULL)
				{
					this->HandleNetworkMessage(np->getMessage());
				}
				SAFE_DELETE(ev);
			}
			if (this->zTimeSinceLastPing > TIMEOUT_VALUE * 2.0f)
			{
				this->CloseConnection("Timeout");
			}
			else if (this->zTimeSinceLastPing > TIMEOUT_VALUE)
			{
				MaloW::Debug("Timeout From Server");
				//Print a Timeout Message to Client
			}
			if(this->zCreated)
			{
				if(this->zWaitTimer >= UPDATE_DELAY)
				{
					this->zWaitTimer = 0.0f;
					this->SendClientUpdate();
				}
			}
			Sleep(5);
		}
	}
	this->zRunning = false;
}

void Client::SendClientUpdate()
{
	std::string msg;
	Vector3 dir = this->zEng->GetCamera()->GetForward();
	Vector3 up = this->zEng->GetCamera()->GetUpVector();
	int position = this->SearchForPlayer(this->zID);
	Vector4 rot = Vector4(0, 0, 0, 0);
	if (position != -1)
	{
		rot = this->zPlayers[position]->GetRotation();
	}
	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_CLIENT_DATA);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_FRAME_TIME, this->zFrameTime);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_DIRECTION, dir.x, dir.y, dir.z);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_UP, up.x, up.y, up.z);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

	this->zServerChannel->sendData(msg);
}

void Client::UpdateCameraPos()
{
	int pos = this->SearchForPlayer(this->zID);
	if (pos != -1)
	{
		Vector3 position = this->zPlayers[pos]->GetPosition();
		position.y += 2.5f;
		this->zEng->GetCamera()->SetPosition(position);
	}
}

void Client::UpdateWorldObjects()
{
	std::vector<Player*>::iterator itp;
	for (itp = this->zPlayers.begin(); itp < this->zPlayers.end(); itp++)
	{
		(*itp)->Update(this->zDeltaTime);
	}

	std::vector<Animal*>::iterator ita;
	for (ita = this->zAnimals.begin(); ita < this->zAnimals.end(); ita++)
	{
		(*ita)->Update(this->zDeltaTime);
	}
}

bool Client::IsAlive()
{
	return this->zRunning;
}

bool Client::CheckKey(const unsigned int ID)
{
	bool result = false;
	char key = this->zKeyInfo.GetKey(ID);
	//Check if key is pressed
	if (this->zEng->GetKeyListener()->IsPressed(key))
	{
		//Check if the Key was pressed last frame
		if (!this->zKeyInfo.GetKeyState(ID))
		{
			std::string msg = "";
			msg = this->zMsgHandler.Convert(MESSAGE_TYPE_KEY_DOWN, ID);

			this->zServerChannel->sendData(msg);
		}
		this->zKeyInfo.SetKeyState(ID, true);
		result = true;
	}
	else 
	{
		//Check if the Key was pressed last frame
		if (this->zKeyInfo.GetKeyState(ID))
		{
			std::string msg = "";
			msg = this->zMsgHandler.Convert(MESSAGE_TYPE_KEY_UP, ID);

			this->zServerChannel->sendData(msg);
		}
		this->zKeyInfo.SetKeyState(ID, false);
		result = false;
	}
	return result;
}

void Client::HandleKeyboardInput()
{
	//float mSpeed = V_WALK_SPEED;
	bool pressed = false;
	
	int pos = this->SearchForPlayer(this->zID);
	if (pos != -1)
	{
		pressed = this->CheckKey(KEY_FORWARD);
		if (!pressed)
		{
			pressed = this->CheckKey(KEY_BACKWARD);
		}

		pressed = this->CheckKey(KEY_LEFT);
		if (!pressed)
		{
			pressed = this->CheckKey(KEY_RIGHT);
		}

		pressed = this->CheckKey(KEY_SPRINT);

		pressed = this->CheckKey(KEY_DUCK);
	}
	else
	{
		MaloW::Debug("Something Went Wrong. This player cannot be found. In function Client::HandleKeyBoardInput");
	}

	if (this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_MENU)))
	{
		this->zServerChannel->sendData(this->zMsgHandler.Convert(MESSAGE_TYPE_CONNECTION_CLOSED, this->zID));
		this->CloseConnection("Escape was pressed");
	}
}

void Client::Ping()
{
	this->zTimeSinceLastPing = 0.0f;
	this->zServerChannel->sendData(this->zMsgHandler.Convert(MESSAGE_TYPE_PING));
}

void Client::HandleNetworkMessage(const std::string& msg)
{
	std::vector<std::string> msgArray;
	msgArray = this->zMsgHandler.SplitMessage(msg);
	char key[1024];
	if(msgArray.size() > 0)
	{
		sscanf_s(msgArray[0].c_str(), "%s ", &key, sizeof(key));

		//Checks what type of message was sent
		if(strcmp(key, PING.c_str()) == 0)
		{
			this->Ping();
		}
		//Player
		else if(strcmp(key, UPDATE_PLAYER.c_str()) == 0)
		{
			this->HandleUpdateObject(msgArray, PLAYER);
		}
		//Animal
		else if(strcmp(key, UPDATE_ANIMAL.c_str()) == 0)
		{
			this->HandleUpdateObject(msgArray, ANIMAL);
		}
		//Static Object
		else if(strcmp(key, UPDATE_STATIC_OBJECT.c_str()) == 0)
		{
			this->HandleUpdateObject(msgArray, STATIC_OBJECT);
		}
		//Static Object
		else if(strcmp(key, NEW_STATIC_OBJECT.c_str()) == 0)
		{
			this->HandleNewObject(msgArray, STATIC_OBJECT);
		}
		//Static Object
		else if(strcmp(key, REMOVE_STATIC_OBJECT.c_str()) == 0)
		{
			this->HandleRemoveObject(msgArray, STATIC_OBJECT);
		}
		//Dynamic Object
		else if(strcmp(key, UPDATE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			this->HandleUpdateObject(msgArray, DYNAMIC_OBJECT);
		}
		//Dynamic Object
		else if(strcmp(key, NEW_DYNAMIC_OBJECT.c_str()) == 0)
		{
			this->HandleNewObject(msgArray, DYNAMIC_OBJECT);
		}
		//Dynamic Object
		else if(strcmp(key, REMOVE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			this->HandleRemoveObject(msgArray, DYNAMIC_OBJECT);
		}
		//Player
		else if(strcmp(key, NEW_PLAYER.c_str()) == 0)
		{
			this->HandleNewObject(msgArray, PLAYER);
		}
		//Animal
		else if(strcmp(key, NEW_ANIMAL.c_str()) == 0)
		{
			this->HandleNewObject(msgArray, ANIMAL);
		}
		//Animal
		else if(strcmp(key, REMOVE_ANIMAL.c_str()) == 0)
		{
			this->HandleRemoveObject(msgArray, ANIMAL);
		}
		//Player
		else if(strcmp(key, REMOVE_PLAYER.c_str()) == 0)
		{
			this->HandleRemoveObject(msgArray, PLAYER);
		}
		else if(strcmp(key, SELF_ID.c_str()) == 0)
		{
			this->zID = this->zMsgHandler.ConvertStringToInt(SELF_ID, msgArray[0]);
			
			Vector3 camDir = this->zEng->GetCamera()->GetForward();
			Vector3 camUp = this->zEng->GetCamera()->GetUpVector();

			std::string serverMessage = "";
			serverMessage = this->zMsgHandler.Convert(MESSAGE_TYPE_USER_DATA);
			serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_MESH_MODEL, this->zMeshID);
			serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_DIRECTION, camDir.x, camDir.y, camDir.z);
			serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_UP, camUp.x, camUp.y, camUp.z);

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
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[0] + " in HandleNetworkMessage");
		}
	}
}

void Client::CloseConnection(const std::string& reason)
{
	MaloW::Debug("Client Shutdown: " + reason);
	//Todo Skriv ut vilket reason som gavs
	this->zServerChannel->Close();
	this->Close();
}

int Client::SearchForPlayer(const int id)
{
	for (unsigned int i = 0; i < this->zPlayers.size(); i++)
	{
		if (this->zPlayers[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;;
}

int Client::SearchForStaticObject(const int id)
{
	for (unsigned int i = 0; i < this->zStaticObjects.size(); i++)
	{
		if (this->zStaticObjects[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

int Client::SearchForDynamicObject( const int id )
{
	for (unsigned int i = 0; i < this->zStaticObjects.size(); i++)
	{
		if (this->zDynamicObjects[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

int Client::SearchForAnimal(const int id)
{
	for (unsigned int i = 0; i < this->zAnimals.size(); i++)
	{
		if (this->zAnimals[i]->GetID() == id)
		{
			return i;
		}
	}
	return -1;
}

int Client::FindObject(const int id, const unsigned int objectType)
{
	int position = -1;
	switch (objectType)
	{
	case PLAYER:
		position = SearchForPlayer(id);
		break;
	case ANIMAL:
		position = SearchForAnimal(id);
		break;
	case STATIC_OBJECT:
		position = SearchForStaticObject(id);
		break;
	case DYNAMIC_OBJECT:
		position = SearchForDynamicObject(id);
		break;
	default:
		position = -1;
		break;
	}
	return position;
}

void Client::HandleNewObject(const std::vector<std::string>& msgArray, const unsigned int objectType )
{
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	Vector3 direction = Vector3(1, 0, 0);
	std::string filename = "";
	int clientID = -1;
	int state = 0;

	switch (objectType)
	{
	case PLAYER:
		clientID = this->zMsgHandler.ConvertStringToInt(NEW_PLAYER, msgArray[0]);
		break;
	case ANIMAL:
		clientID = this->zMsgHandler.ConvertStringToInt(NEW_ANIMAL, msgArray[0]);
		break;
	case STATIC_OBJECT:
		clientID = this->zMsgHandler.ConvertStringToInt(NEW_STATIC_OBJECT, msgArray[0]);
		break;
	case DYNAMIC_OBJECT:
		clientID = this->zMsgHandler.ConvertStringToInt(NEW_DYNAMIC_OBJECT, msgArray[0]);
		break;
	default:
		break;
	}
	char key[512];
	for(unsigned int i = 1; i < msgArray.size(); i++)
	{
		sscanf_s(msgArray[i].c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, NEW_PLAYER.c_str()) == 0)
		{
			
		}
		else if(strcmp(key, POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(POSITION, msgArray[i]);
		}
		else if(strcmp(key, ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(ROTATION, msgArray[i]);
		}
		else if(strcmp(key, STATE.c_str()) == 0)
		{
			state = this->zMsgHandler.ConvertStringToInt(STATE, msgArray[i]);
		}
		else if(strcmp(key, SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(SCALE, msgArray[i]);
		}
		else if(strcmp(key, MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(MESH_MODEL, msgArray[i]);
		}
		else if (strcmp(key, DIRECTION.c_str()) == 0)
		{
			direction = this->zMsgHandler.ConvertStringToVector(DIRECTION, msgArray[i]);
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[i] + " in HandleNewObject");
		}
	}
	if (!this->zCreated)
	{
		if (clientID == this->zID)
		{
			this->zCreated = true;
		}
	}
	if (clientID != -1)
	{
		int pos;
		switch (objectType)
		{
		case PLAYER:
			pos = this->SearchForPlayer(clientID);
			break;
		case ANIMAL:
			pos = this->SearchForAnimal(clientID);
			break;
		case STATIC_OBJECT:
			pos = this->SearchForStaticObject(clientID);
			break;
		case DYNAMIC_OBJECT:
			pos = this->SearchForStaticObject(clientID);
			break;
		default:
			pos = -1;
			break;
		}
		if (pos == -1)
		{
			WorldObject* newWorldObject = NULL;
			switch (objectType)
			{
			case PLAYER:
				newWorldObject = new Player();
				break;
			case ANIMAL:
				newWorldObject = new Animal();
				break;
			case STATIC_OBJECT:
				newWorldObject = new StaticObject();
				break;
			case DYNAMIC_OBJECT:
				newWorldObject = new StaticObject();
				break;
			default:
				break;
			}
			//Creates a StaticMesh from the given Filename
			iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
			mesh->SetQuaternion(rotation);
			mesh->Scale(scale);
		
			//Create player data
			newWorldObject->AddStaticMesh(mesh);
			newWorldObject->SetID(clientID);
			switch (objectType)
			{
			case PLAYER:
				dynamic_cast<Player*>(newWorldObject)->SetNextPosition(position);
				dynamic_cast<Player*>(newWorldObject)->SetState(state);
				this->zPlayers.push_back(dynamic_cast<Player*>(newWorldObject));
				break;
			case STATIC_OBJECT:
				this->zStaticObjects.push_back(dynamic_cast<StaticObject*>(newWorldObject));
				break;
			case DYNAMIC_OBJECT:
				//Implement here
				break;
			case ANIMAL:
				dynamic_cast<Animal*>(newWorldObject)->SetNextPosition(position);
				dynamic_cast<Animal*>(newWorldObject)->SetState(state);
				this->zAnimals.push_back(dynamic_cast<Animal*>(newWorldObject));
				break;
			default:
				break;
			}
		}
		else
		{
			MaloW::Debug("Cant create a new Object. It already exists");
		}
	}
}

void Client::HandleRemoveObject(const std::vector<std::string>& msgArray, const unsigned int objectType )
{
	int ID = -1;
	int pos = -1;
	switch(objectType)
	{
	case PLAYER:
		ID = this->zMsgHandler.ConvertStringToInt(REMOVE_PLAYER, msgArray[0]);
		pos = SearchForPlayer(ID);
		break;
	case ANIMAL:
		ID = this->zMsgHandler.ConvertStringToInt(REMOVE_ANIMAL, msgArray[0]);
		pos = SearchForAnimal(ID);
		break;
	case STATIC_OBJECT:
		ID = this->zMsgHandler.ConvertStringToInt(REMOVE_STATIC_OBJECT, msgArray[0]);
		pos = SearchForStaticObject(ID);
		break;
	case DYNAMIC_OBJECT:
		ID = this->zMsgHandler.ConvertStringToInt(REMOVE_DYNAMIC_OBJECT, msgArray[0]);
		pos = SearchForDynamicObject(ID);
		break;
	default:
		break;
	}
	//Check if client was found in the array
	if(pos != -1)
	{
		if(ID == this->zID)
		{
			this->CloseConnection("Unknown reason possible Kicked");
		}
		switch(objectType)
		{
		case PLAYER:
			this->zEng->DeleteMesh(this->zPlayers[pos]->GetMesh());
			this->zPlayers.erase(zPlayers.begin() + pos);
			break;
		case ANIMAL:
			this->zEng->DeleteMesh(this->zAnimals[pos]->GetMesh());
			this->zAnimals.erase(zAnimals.begin() + pos);
			break;
		case STATIC_OBJECT:
			this->zEng->DeleteMesh(this->zStaticObjects[pos]->GetMesh());
			this->zStaticObjects.erase(zStaticObjects.begin() + pos);
			break;
		case DYNAMIC_OBJECT:
			this->zEng->DeleteMesh(this->zDynamicObjects[pos]->GetMesh());
			this->zDynamicObjects.erase(zDynamicObjects.begin() + pos);
			break;
		default:
			break;
		}
	}
}

void Client::HandleUpdateObject(const std::vector<std::string>& msgArray, const unsigned int objectType)
{
	int ID = -1;
	int pos = -1;
	//Get ID and Position Depending on type
	switch (objectType)
	{
	case PLAYER:
		ID = this->zMsgHandler.ConvertStringToInt(UPDATE_PLAYER, msgArray[0]);
		pos = SearchForPlayer(ID);
		break;
	case ANIMAL:
		ID = this->zMsgHandler.ConvertStringToInt(UPDATE_ANIMAL, msgArray[0]);
		pos = SearchForAnimal(ID);
		break;
	case STATIC_OBJECT:
		ID = this->zMsgHandler.ConvertStringToInt(UPDATE_STATIC_OBJECT, msgArray[0]);
		pos = SearchForStaticObject(ID);
		break;
	case DYNAMIC_OBJECT:
		ID = this->zMsgHandler.ConvertStringToInt(UPDATE_DYNAMIC_OBJECT, msgArray[0]);
		pos = SearchForDynamicObject(ID);
		break;
	default:
		pos = -1;
		MaloW::Debug("Wrong ObjectType in Client::HandleUpdate()");
		break;
	}
	//Check if object was found
	if(pos != -1)
	{
		WorldObject* worldObjectPointer = NULL;
		switch (objectType)
		{
		case PLAYER:
			worldObjectPointer = this->zPlayers[pos];
			break;
		case ANIMAL:
			worldObjectPointer = this->zAnimals[pos];
			break;
		case STATIC_OBJECT:
			worldObjectPointer = this->zStaticObjects[pos];
			break;
		case DYNAMIC_OBJECT:
			worldObjectPointer = this->zDynamicObjects[pos];
			break;
		default:
			break;
		}
		Vector3 position;
		Vector4 rotation;
		int state;
		std::string filename;
		float serverTime = 0.0f;

		bool bState = false;
		bool bPos = false;
		bool bRot = false;
		bool bFile = false;
		bool bTime = false;
		if (worldObjectPointer != NULL)
		{
			char key[512];
			worldObjectPointer->SetID(ID);
			for(unsigned int i = 1; i < msgArray.size(); i++)
			{
				sscanf_s(msgArray[i].c_str(), "%s ", &key, sizeof(key));

				if(strcmp(key, POSITION.c_str()) == 0)
				{
					position = this->zMsgHandler.ConvertStringToVector(POSITION, msgArray[i]);
					
					bPos = true;
				}
				else if(strcmp(key, ROTATION.c_str()) == 0)
				{
					rotation = this->zMsgHandler.ConvertStringToQuaternion(ROTATION, msgArray[i]);

					bRot = true;
				}
				else if(strcmp(key, STATE.c_str()) == 0)
				{
					state = this->zMsgHandler.ConvertStringToInt(STATE, msgArray[i]);

					bState = true;
				}
				else if(strcmp(key, FRAME_TIME.c_str()) == 0)
				{
					bTime = true;
					serverTime = this->zMsgHandler.ConvertStringToFloat(FRAME_TIME, msgArray[i]);
				}
				else if(strcmp(key, MESH_MODEL.c_str()) == 0)
				{
					bFile = true;
					filename = this->zMsgHandler.ConvertStringToSubstring(MESH_MODEL, msgArray[i]);
				}
				else
				{
					MaloW::Debug("C: Unknown Message Was sent from server -" + msgArray[i] + "- in HandleUpdateObject");
				}
			}

			if (bPos)
			{
				switch (objectType)
				{
				case PLAYER:
					dynamic_cast<Player*>(worldObjectPointer)->SetNextPosition(position);
					break;
				case ANIMAL:
					dynamic_cast<Animal*>(worldObjectPointer)->SetNextPosition(position);
					break;
				case STATIC_OBJECT:
					worldObjectPointer->SetPosition(position);
					break;
				case DYNAMIC_OBJECT:
					dynamic_cast<DynamicObject*>(worldObjectPointer)->SetPosition(position);
					break;
				default:
					break;
				}
			}

			if (bRot)
			{
				worldObjectPointer->SetRotation(rotation);
			}

			if (bState)
			{
				switch (objectType)
				{
				case PLAYER:
					dynamic_cast<Player*>(worldObjectPointer)->SetState(state);
					break;
				case ANIMAL:
					dynamic_cast<Animal*>(worldObjectPointer)->SetState(state);
					break;
				case DYNAMIC_OBJECT:
					dynamic_cast<DynamicObject*>(worldObjectPointer)->SetState(state);
					break;
				default:
					break;
				}
			}
			if (bFile)
			{
				//Create a new Mesh with the current values
				iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), worldObjectPointer->GetPosition());
				float scale = worldObjectPointer->GetScale().y;
				Vector4 quat = worldObjectPointer->GetRotation();

				mesh->Scale(scale);
				mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
				if (worldObjectPointer->HasMesh())
				{
					this->zEng->DeleteMesh(worldObjectPointer->GetMesh());
				}
				worldObjectPointer->AddStaticMesh(mesh);
			}

			//Copy over new Values to the vector
			switch (objectType)
			{
			case PLAYER:
				this->zPlayers[pos] = dynamic_cast<Player*>(worldObjectPointer);
				break;
			case ANIMAL:
				this->zAnimals[pos] = dynamic_cast<Animal*>(worldObjectPointer);
				break;
			case STATIC_OBJECT:
				this->zStaticObjects[pos] = dynamic_cast<StaticObject*>(worldObjectPointer);
				break;
			case DYNAMIC_OBJECT:
				this->zDynamicObjects[pos] = dynamic_cast<DynamicObject*>(worldObjectPointer);
				break;
			default:
				break;
			}
		}
	}
}
