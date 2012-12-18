#include "GameFiles/ClientSide/Client.h"
#include "Graphics.h"
#include "Safe.h"
#include "Network/NetworkPacket.h"

using namespace MaloW;

//Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;
// 30 updates per sec
static const float UPDATE_DELAY = 0.0333f;
static const float CIRCULAR_GUI_DISPLAY_TIMER = 2.0f;
static const float MAX_DISTANCE_TO_OBJECT = 50.0f;

Client::Client()
{
	this->zID = 0;
	this->zIP = "";
	this->zPort = 0;
	this->zEng = NULL;
	this->zRunning = true;
	this->zCreated = false;
	this->zFrameTime = 0.0f;
	this->zServerChannel = NULL;
	this->zCircularGuiShowTimer = 0.0f;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	this->zTimeSinceLastPing = 0.0f;
	this->zMeshID = "Media/scale.obj";
	this->zSendUpdateDelayTimer = 0.0f;
	this->zMsgHandler = NetworkMessageConverter();

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

	zGui->RemoveFromRenderer(this->zEng);
	if(this->zGui)
	{
		delete this->zGui;
		this->zGui = 0;
	}

	if(this->zServerChannel)
	{
		delete this->zServerChannel;
		this->zServerChannel = 0;
	}
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

	int x = this->zEng->GetEngineParameters()->windowHeight / 4;
	int y = this->zEng->GetEngineParameters()->windowWidth / 4;

	zGui = new CircularListGui(x, y, x*2, y*2, "Media/Fern_02_v01.png");
	zGui->AddToRenderer(this->zEng);
	this->zEng->StartRendering();
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

		this->zSendUpdateDelayTimer += this->zDeltaTime;
		this->zTimeSinceLastPing += this->zDeltaTime;

		if(this->zCreated)
		{
			this->HandleKeyboardInput();

			if(this->zSendUpdateDelayTimer >= UPDATE_DELAY)
			{
				this->zSendUpdateDelayTimer = 0.0f;
				this->SendClientUpdate();
			}
			this->UpdateCameraPos();

			this->UpdateWorldObjects();
		}
		if (this->stayAlive)
		{
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
	int position = this->zObjectManager.SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
	Vector4 rot = Vector4(0, 0, 0, 0);
	if (position != -1)
	{
		rot = this->zObjectManager.GetPlayer(position)->GetRotation();
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
	int pos = this->zObjectManager.SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
	if (pos != -1)
	{
		Vector3 position = this->zObjectManager.GetPlayer(pos)->GetPosition();
		position.y += 2.5f;
		this->zEng->GetCamera()->SetPosition(position);
	}
}

void Client::UpdateWorldObjects()
{
	this->zObjectManager.UpdateObjects(this->zDeltaTime);
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
	bool pressed = false;
	
	int pos = this->zObjectManager.SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
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

		if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INTERACT)))
		{
			this->RayVsWorld();
			this->zCircularGuiShowTimer = CIRCULAR_GUI_DISPLAY_TIMER;
			if (zGui->IsHidden())
			{
				zGui->ShowGui();
			}
		}
		else
		{
			if (this->zCircularGuiShowTimer > 0.0f)
			{
				this->zCircularGuiShowTimer -= this->zDeltaTime;
				float fadeValue = this->zDeltaTime / CIRCULAR_GUI_DISPLAY_TIMER;
				this->zGui->FadeOut(fadeValue);
			}
			else
			{
				if (!zGui->IsHidden())
				{
					zGui->HideGui();
				}
			}
		}
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
		if(strcmp(key, M_PING.c_str()) == 0)
		{
			this->Ping();
		}
		//Player
		else if(strcmp(key, M_UPDATE_PLAYER.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_PLAYER, msgArray[0]);
			this->HandleUpdateObject(msgArray, OBJECT_TYPE_PLAYER, id);
		}
		//Animal
		else if(strcmp(key, M_UPDATE_ANIMAL.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_ANIMAL, msgArray[0]);
			this->HandleUpdateObject(msgArray, OBJECT_TYPE_ANIMAL, id);
		}
		//Static Object
		else if(strcmp(key, M_UPDATE_STATIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_STATIC_OBJECT, msgArray[0]);
			this->HandleUpdateObject(msgArray, OBJECT_TYPE_STATIC_OBJECT, id);
		}
		//Static Object
		else if(strcmp(key, M_NEW_STATIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_STATIC_OBJECT, msgArray[0]);
			this->HandleNewObject(msgArray, OBJECT_TYPE_STATIC_OBJECT, id);
		}
		//Static Object
		else if(strcmp(key, M_REMOVE_STATIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_STATIC_OBJECT, msgArray[0]);
			this->HandleRemoveObject(msgArray, OBJECT_TYPE_STATIC_OBJECT, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_UPDATE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_DYNAMIC_OBJECT, msgArray[0]);
			this->HandleUpdateObject(msgArray, OBJECT_TYPE_DYNAMIC_OBJECT, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_NEW_DYNAMIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_DYNAMIC_OBJECT, msgArray[0]);
			this->HandleNewObject(msgArray, OBJECT_TYPE_DYNAMIC_OBJECT, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_REMOVE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_DYNAMIC_OBJECT, msgArray[0]);
			this->HandleRemoveObject(msgArray, OBJECT_TYPE_DYNAMIC_OBJECT, id);
		}
		//Player
		else if(strcmp(key, M_NEW_PLAYER.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_PLAYER, msgArray[0]);
			this->HandleNewObject(msgArray, OBJECT_TYPE_PLAYER, id);
		}
		//Animal
		else if(strcmp(key, M_NEW_ANIMAL.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_ANIMAL, msgArray[0]);
			this->HandleNewObject(msgArray, OBJECT_TYPE_ANIMAL, id);
		}
		//Animal
		else if(strcmp(key, M_REMOVE_ANIMAL.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_ANIMAL, msgArray[0]);
			this->HandleRemoveObject(msgArray, OBJECT_TYPE_ANIMAL, id);
		}
		//Player
		else if(strcmp(key, M_REMOVE_PLAYER.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_PLAYER, msgArray[0]);
			this->HandleRemoveObject(msgArray, OBJECT_TYPE_PLAYER, id);
		}
		else if(strcmp(key, M_ADD_INVENTORY_ITEM.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ADD_INVENTORY_ITEM, msgArray[0]);
			this->HandleAddInventoryItem(msgArray, id);
		}
		else if(strcmp(key, M_REMOVE_INVENTORY_ITEM.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_INVENTORY_ITEM, msgArray[0]);
			this->HandleRemoveInventoryItem(msgArray, id);
		}
		else if(strcmp(key, M_SELF_ID.c_str()) == 0)
		{
			this->zID = this->zMsgHandler.ConvertStringToInt(M_SELF_ID, msgArray[0]);
			
			Vector3 camDir = this->zEng->GetCamera()->GetForward();
			Vector3 camUp = this->zEng->GetCamera()->GetUpVector();

			std::string serverMessage = "";
			serverMessage = this->zMsgHandler.Convert(MESSAGE_TYPE_USER_DATA);
			serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_MESH_MODEL, this->zMeshID);
			serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_DIRECTION, camDir.x, camDir.y, camDir.z);
			serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_UP, camUp.x, camUp.y, camUp.z);

			this->zServerChannel->sendData(serverMessage);
		}
		else if(strcmp(key, M_SERVER_FULL.c_str()) == 0)
		{
			this->CloseConnection("Server is full");
		}
		else if(strcmp(key, M_KICKED.c_str()) == 0)
		{
			this->CloseConnection("You got kicked");
		}
		else if(strcmp(key, M_SERVER_SHUTDOWN.c_str()) == 0)
		{
			this->CloseConnection("Server Shutdown");
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[0] + " in HandleNetworkMessage");
		}
	}
}

void Client::HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id)
{
	std::string itemName;
	unsigned int itemWeight;
	unsigned int itemType = -1;
	float weaponDamage;
	float weaponRange;
	float hunger;
	char key[256];
	for(unsigned int i = 1; i < msgArray.size(); i++)
	{
		sscanf_s(msgArray[i].c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			itemName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, msgArray[i]);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			itemWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, msgArray[i]);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, msgArray[i]);
		}
		else if(strcmp(key, M_WEAPON_DAMAGE.c_str()) == 0)
		{
			weaponDamage = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_DAMAGE, msgArray[i]);
		}
		else if(strcmp(key, M_WEAPON_RANGE.c_str()) == 0)
		{
			weaponRange = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_RANGE, msgArray[i]);
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, msgArray[i]);
		}
		if (itemType == -1)
		{
			MaloW::Debug("Wrong or no Item Type sent from server in Client::HandleAddInventoryItem ItemType: " + MaloW::convertNrToString(itemType));
			return;
		}
		//Todo add more data to item to identify type ex Bow/Axe/Pocket Knife
		Item* item = 0;
		switch (itemType)
		{
		case ITEM_TYPE_FOOD_MEAT:
			item = new Food(id, itemWeight, itemName, itemType, hunger);
			break;
		case ITEM_TYPE_WEAPON_RANGED_BOW:
			item = new RangedWeapon(id, itemWeight, itemName, itemType, weaponDamage, weaponRange);
			break;
		case ITEM_TYPE_WEAPON_RANGED_ROCK:
			item = new RangedWeapon(id, itemWeight, itemName, itemType, weaponDamage, weaponRange);
			break;
		case ITEM_TYPE_WEAPON_MELEE_AXE:
			item = new MeleeWeapon(id, itemWeight, itemName, itemType, weaponDamage, weaponRange);
			break;
		case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
			item = new MeleeWeapon(id, itemWeight, itemName, itemType, weaponDamage, weaponRange);
			break;
		case ITEM_TYPE_GEAR_HEAD:
			item = new Gear(id, itemWeight, itemName, itemType);
			break;
		case ITEM_TYPE_GEAR_CHEST:
			item = new Gear(id, itemWeight, itemName, itemType);
			break;
		case ITEM_TYPE_GEAR_LEGS:
			item = new Gear(id, itemWeight, itemName, itemType);
			break;
		case ITEM_TYPE_GEAR_BOOTS:
			item = new Gear(id, itemWeight, itemName, itemType);
			break;
		default:
			break;
		}

	}
}

void Client::HandleRemoveInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id)
{
	//Remove Item with ITEM_ID = id
}

void Client::CloseConnection(const std::string& reason)
{
	MaloW::Debug("Client Shutdown: " + reason);
	//Todo Skriv ut vilket reason som gavs
	this->zServerChannel->Close();
	this->Close();
}

void Client::HandleNewObject(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id)
{
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	Vector3 direction = Vector3(1, 0, 0);
	std::string filename = "";
	int state = 0;

	char key[512];
	for(unsigned int i = 1; i < msgArray.size(); i++)
	{
		sscanf_s(msgArray[i].c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, msgArray[i]);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, msgArray[i]);
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			state = this->zMsgHandler.ConvertStringToInt(M_STATE, msgArray[i]);
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, msgArray[i]);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, msgArray[i]);
		}
		else if (strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			direction = this->zMsgHandler.ConvertStringToVector(M_DIRECTION, msgArray[i]);
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[i] + " in HandleNewObject");
		}
	}
	if (!this->zCreated)
	{
		if (id == this->zID)
		{
			this->zCreated = true;
		}
	}
	if (id != -1)
	{
		int pos = this->zObjectManager.SearchForObject(objectType, id);

		if (pos == -1)
		{
			WorldObject* newWorldObject = NULL;
			switch (objectType)
			{
			case OBJECT_TYPE_PLAYER:
				newWorldObject = new PlayerObject(id);
				break;
			case OBJECT_TYPE_ANIMAL:
				newWorldObject = new AnimalObject(id);
				break;
			case OBJECT_TYPE_STATIC_OBJECT:
				newWorldObject = new StaticObject(id);
				break;
			case OBJECT_TYPE_DYNAMIC_OBJECT:
				newWorldObject = new StaticObject(id);
				break;
			default:
				return;
				break;
			}
			//Creates a StaticMesh from the given Filename
			iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
			mesh->SetQuaternion(rotation);
			mesh->Scale(scale);
		
			//Create player data
			newWorldObject->SetStaticMesh(mesh);
			newWorldObject->SetID(id);
			switch (objectType)
			{
			case OBJECT_TYPE_PLAYER:
				dynamic_cast<PlayerObject*>(newWorldObject)->SetNextPosition(position);
				dynamic_cast<PlayerObject*>(newWorldObject)->SetState(state);
				this->zObjectManager.AddObject(dynamic_cast<PlayerObject*>(newWorldObject));
				break;
			case OBJECT_TYPE_STATIC_OBJECT:
				this->zObjectManager.AddObject(dynamic_cast<StaticObject*>(newWorldObject));
				break;
			case OBJECT_TYPE_DYNAMIC_OBJECT:
				this->zObjectManager.AddObject(dynamic_cast<DynamicObject*>(newWorldObject));
				break;
			case OBJECT_TYPE_ANIMAL:
				dynamic_cast<AnimalObject*>(newWorldObject)->SetNextPosition(position);
				dynamic_cast<AnimalObject*>(newWorldObject)->SetState(state);
				this->zObjectManager.AddObject(dynamic_cast<AnimalObject*>(newWorldObject));
				break;
			default:
				MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleRemoveObject");
				break;
			}
		}
		else
		{
			MaloW::Debug("Cant create a new Object. It already exists");
		}
	}
}

void Client::HandleRemoveObject(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id)
{
	int pos = -1;

	if (id != -1)
	{
		pos = this->zObjectManager.SearchForObject(objectType, id);

		//Check if client was found in the array
		if(pos != -1)
		{
			if(id == this->zID)
			{
				this->CloseConnection("Unknown reason possible Kicked");
			}
			iMesh* mesh = 0;
			switch(objectType)
			{
			case OBJECT_TYPE_PLAYER:
				mesh = this->zObjectManager.GetPlayer(pos)->GetMesh();
				break;
			case OBJECT_TYPE_ANIMAL:
				mesh = this->zObjectManager.GetAnimal(pos)->GetMesh();
				break;
			case OBJECT_TYPE_STATIC_OBJECT:
				mesh = this->zObjectManager.GetStaticObject(pos)->GetMesh();
				break;
			case OBJECT_TYPE_DYNAMIC_OBJECT:
				mesh = this->zObjectManager.GetDynamicObject(pos)->GetMesh();
				break;
			default:
				MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleRemoveObject");
				return;
				break;
			}
			if(mesh)
			{
				this->zEng->DeleteMesh(mesh);
			}
			this->zObjectManager.SearchAndRemove(objectType, id);
		}
	}
}

void Client::HandleUpdateObject(const std::vector<std::string>& msgArray, const unsigned int objectType, const unsigned int id)
{
	int pos;
	//Get ID and Position Depending on type
	if (id != -1)
	{
		pos = this->zObjectManager.SearchForObject(objectType, id);
		//Check if object was found
		if(pos != -1)
		{
			WorldObject* worldObjectPointer = NULL;
			switch (objectType)
			{
			case OBJECT_TYPE_PLAYER:
				worldObjectPointer = this->zObjectManager.GetPlayer(pos);
				break;
			case OBJECT_TYPE_ANIMAL:
				worldObjectPointer = this->zObjectManager.GetAnimal(pos);
				break;
			case OBJECT_TYPE_STATIC_OBJECT:
				worldObjectPointer = this->zObjectManager.GetStaticObject(pos);
				break;
			case OBJECT_TYPE_DYNAMIC_OBJECT:
				worldObjectPointer = this->zObjectManager.GetDynamicObject(pos);
				break;
			default:
				MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleUpdateObject");
				return;
				break;
			}
			Vector3 position;
			Vector4 rotation;
			int state;
			std::string filename;
			float health;
			float hunger;
			float hydration;
			float stamina;
			float serverTime = 0.0f;

			bool bFile = false;
			bool bHunger = false;
			bool bHealth = false;
			bool bStamina = false;
			bool bHydration = false;
			if (worldObjectPointer != NULL)
			{
				char key[512];
				worldObjectPointer->SetID(id);
				for(unsigned int i = 1; i < msgArray.size(); i++)
				{
					sscanf_s(msgArray[i].c_str(), "%s ", &key, sizeof(key));

					if(strcmp(key, M_POSITION.c_str()) == 0)
					{
						position = this->zMsgHandler.ConvertStringToVector(M_POSITION, msgArray[i]);
					}
					else if(strcmp(key, M_ROTATION.c_str()) == 0)
					{
						rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, msgArray[i]);
					}
					else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
					{
						serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, msgArray[i]);
					}
					else if(strcmp(key, M_STATE.c_str()) == 0)
					{
						state = this->zMsgHandler.ConvertStringToInt(M_STATE, msgArray[i]);
					}
					else if(strcmp(key, M_HEALTH.c_str()) == 0)
					{
						health = this->zMsgHandler.ConvertStringToFloat(M_HEALTH, msgArray[i]);
						bHealth = true;
					}
					else if(strcmp(key, M_HUNGER.c_str()) == 0)
					{
						hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, msgArray[i]);
						bHunger = true;
					}
					else if(strcmp(key, M_HYDRATION.c_str()) == 0)
					{
						hydration = this->zMsgHandler.ConvertStringToFloat(M_HYDRATION, msgArray[i]);
						bHydration = true;
					}
					else if(strcmp(key, M_STAMINA.c_str()) == 0)
					{
						stamina = this->zMsgHandler.ConvertStringToFloat(M_STAMINA, msgArray[i]);
						bStamina = true;
					}
					else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
					{
						bFile = true;
						filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, msgArray[i]);
					}
					else
					{
						MaloW::Debug("Client: Unknown Message Was sent from server - " + msgArray[i] + " - in HandleUpdateObject");
					}
				}
				switch (objectType)
				{
				case OBJECT_TYPE_PLAYER:
					dynamic_cast<PlayerObject*>(worldObjectPointer)->SetNextPosition(position);
					dynamic_cast<PlayerObject*>(worldObjectPointer)->SetState(state);
					break;
				case OBJECT_TYPE_ANIMAL:
					dynamic_cast<AnimalObject*>(worldObjectPointer)->SetNextPosition(position);
					dynamic_cast<AnimalObject*>(worldObjectPointer)->SetState(state);
					break;
				case OBJECT_TYPE_STATIC_OBJECT:
					worldObjectPointer->SetPosition(position);
					break;
				case OBJECT_TYPE_DYNAMIC_OBJECT:
					dynamic_cast<DynamicObject*>(worldObjectPointer)->SetNextPosition(position);
					dynamic_cast<DynamicObject*>(worldObjectPointer)->SetState(state);
					break;
				default:
					MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleUpdateObject");
					return;
					break;
				}
				worldObjectPointer->SetRotation(rotation);

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
					worldObjectPointer->SetStaticMesh(mesh);
				}
				if (bHydration)
				{
					switch (objectType)
					{
					case OBJECT_TYPE_PLAYER:
						dynamic_cast<PlayerObject*>(worldObjectPointer)->SetHydration(hydration);
						break;
					default:
						MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleUpdateObject");
						return;
						break;
					}
				}
				if (bHunger)
				{
					switch (objectType)
					{
					case OBJECT_TYPE_PLAYER:
						dynamic_cast<PlayerObject*>(worldObjectPointer)->SetHunger(hunger);
						break;
					default:
						MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleUpdateObject");
						return;
						break;
					}
				}
				if (bHealth)
				{
					switch (objectType)
					{
					case OBJECT_TYPE_PLAYER:
						dynamic_cast<PlayerObject*>(worldObjectPointer)->SetHealth(health);
						break;
					default:
						MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleUpdateObject");
						return;
						break;
					}
				}
				if (bStamina)
				{
					switch (objectType)
					{
					case OBJECT_TYPE_PLAYER:
						dynamic_cast<PlayerObject*>(worldObjectPointer)->SetStamina(stamina);
						break;
					default:
						MaloW::Debug("Cant Translate type: " + MaloW::convertNrToString(objectType) + " In Client::HandleUpdateObject");
						return;
						break;
					}
				}
			}
		}
	}
}

std::vector<StaticObject*> Client::RayVsWorld()
{
	Vector3 origin = this->zEng->GetCamera()->GetPosition();
	Vector3 camForward = this->zEng->GetCamera()->GetForward();
	CollisionData data;
	std::vector<StaticObject*> static_Collisions;
	//Static objects
	std::vector<StaticObject*> staticObjects = this->zObjectManager.GetStaticObjects();
	for(auto it = staticObjects.begin(); it < staticObjects.end(); it++)
	{
		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMesh(origin, camForward, (*it)->GetMesh());

		if (data.collision &&  data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			static_Collisions.push_back((*it));
		}
	}

	return static_Collisions;
}

void Client::CheckCollision()
{
	int position = this->zObjectManager.SearchForObject(OBJECT_TYPE_PLAYER, this->zID);

	if (position != -1)
	{
		iMesh* playerMesh = this->zObjectManager.GetPlayer(position)->GetMesh();

		CollisionData data;
	}
}

void Client::SendPickupItemMessage(unsigned int id)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_PICKUP_ITEM, id);

	this->zServerChannel->sendData(msg);
}

void Client::SendDropItemMessage(unsigned int id)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_DROP_ITEM, id);

	this->zServerChannel->sendData(msg);
}