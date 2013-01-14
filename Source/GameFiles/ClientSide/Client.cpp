#include "GameFiles/ClientSide/Client.h"
#include "Graphics.h"
#include "Safe.h"
#include "Network/NetworkPacket.h"
#include "ClientServerMessages.h"

using namespace MaloW;

//Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;
// 30 updates per sec
static const float UPDATE_DELAY = 0.0333f;
static const float MAX_DISTANCE_TO_OBJECT = 10.0f;

Client::Client()
{
	Messages::ClearDebug();

	this->zID = 0;
	this->zIP = "";
	this->zPort = 0;
	this->zIsHuman = true;
	this->zRunning = true;
	this->zCreated = false;
	this->zShowCursor = false;
	this->zFrameTime = 0.0f;
	this->zTimeSinceLastPing = 0.0f;
	this->zMeshID = "Media/Bow_v01.obj";
	this->zSendUpdateDelayTimer = 0.0f;

	this->zEng = NULL;
	this->zGuiManager = NULL;
	this->zObjectManager = NULL;
	this->zServerChannel = NULL;
	this->zPlayerInventory = NULL;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	
	this->zMsgHandler = NetworkMessageConverter();
}

int Client::Connect(const std::string& ip, const int port)
{
	int code = 0;

	this->zIP = ip;
	this->zPort = port;

	this->zServerChannel = new ServerChannel();
	//Tries to Connect to a server with the specified Ip and Port
	code = this->zServerChannel->InitConnection(ip, port);
	
	return code;
}

Client::~Client()
{
	this->Close();
	this->WaitUntillDone();

	if (this->zGuiManager)
	{
		delete this->zGuiManager;
		this->zGuiManager = NULL;
	}

	if (this->zObjectManager)
	{
		delete this->zObjectManager;
		this->zObjectManager = NULL;
	}

	if (this->zServerChannel)
	{
		delete this->zServerChannel;
		this->zServerChannel = NULL;
	}

	if (this->zPlayerInventory)
	{
		delete this->zPlayerInventory;
		this->zPlayerInventory = NULL;
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

	this->zGuiManager->Update(this->zDeltaTime);

	return this->zDeltaTime;
}

void Client::InitGraphics()
{
	this->zEng->CreateSkyBox("Media/skymap.dds");
	this->zEng->GetCamera()->SetPosition( Vector3(1, 4, -1) );
	this->zEng->GetCamera()->LookAt( Vector3(0, 0, 0) );

	
	//iTerrain* terrain = this->zEng->CreateTerrain(Vector3(0, 0, 0), Vector3(10, 10, 10), 20);

	//this->zObjectManager->AddTerrain(terrain);
	//this->zEng->StartRendering();
}

void Client::Init()
{
	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);

	this->zEng = GetGraphics();

	this->zObjectManager = new WorldObjectManager();
	this->zGuiManager = new GuiManager(this->zEng);
	this->zPlayerInventory = new Inventory();

	Sleep(1000);
	this->InitGraphics();
}

void Client::Life()
{
	MaloW::Debug("Client Process Started");
	this->zServerChannel->setNotifier(this);

	this->zServerChannel->Start();

	this->Init();

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
			this->ReadMessage();

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

void Client::ReadMessage()
{
	static const unsigned int MAX_NR_OF_MESSAGES = 10;

	int nrOfMessages = this->GetEventQueueSize();
	if (nrOfMessages == 0)
		return;

	int messages_To_Read = min(MAX_NR_OF_MESSAGES, nrOfMessages);
	for (int i = 0; i < messages_To_Read; i++)
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
	}
}

void Client::SendClientUpdate()
{
	std::string msg;
	Vector3 dir = this->zEng->GetCamera()->GetForward();
	Vector3 up = this->zEng->GetCamera()->GetUpVector();
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
	Vector4 rot = Vector4(0, 0, 0, 0);
	if (index != -1)
	{
		rot = this->zObjectManager->GetPlayerObject(index)->GetRotation();
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
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
	if (index != -1)
	{
		Vector3 position = this->zObjectManager->GetPlayerObject(index)->GetPosition();
		iTerrain* terrain = this->zObjectManager->GetTerrain();
		if (terrain)
		{
			position.y = terrain->GetYPositionAt(position.x, position.z);
			terrain = NULL;
		}
		position.y += 2.5f;
		this->zEng->GetCamera()->SetPosition(position);
	}
}

void Client::UpdateWorldObjects()
{
	this->zObjectManager->UpdateObjects(this->zDeltaTime);
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

void Client::CheckMovementKeys()
{
	bool pressed = this->CheckKey(KEY_FORWARD);
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

void Client::HandleKeyboardInput()
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
	if (index == -1)
	{
		MaloW::Debug("Something Went Wrong. This player cannot be found. In function Client::HandleKeyBoardInput");
		return;
	}

	zShowCursor = this->zGuiManager->IsGuiOpen();

	this->CheckMovementKeys();

	Menu_select_data msd;
	if(zShowCursor)
	{
		msd = this->zGuiManager->CheckCollisionInv(); // Returns -1 on both values if no hits.

		if (msd.zAction != -1)
		{
			if (msd.zID != -1)
			{
				if (msd.zAction == USE)
				{
					Item* item = this->zPlayerInventory->SearchAndGetItem(msd.zID);

					if (item)
					{
						std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_USE, item->GetID());	
						this->zServerChannel->sendData(msg);
					}
				}
				else if (msd.zAction == DROP)
				{
					this->SendDropItemMessage(msd.zID);
				}
			}
		}
	}

	//Used For Testing ATM
	//if (this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_JUMP)))
	//{
	//	if (!this->zKeyInfo.GetKeyState(KEY_JUMP))
	//	{
	//		if (this->zPlayerInventory->GetItems().size() > 0)
	//		{
	//			int id = this->zPlayerInventory->GetItem(0)->GetID();
	//			this->SendDropItemMessage(id);
	//		}
	//	}
	//	this->zKeyInfo.SetKeyState(KEY_JUMP, true);
	//}
	//else
	//{
	//	this->zKeyInfo.SetKeyState(KEY_JUMP, false);
	//}
	//Used For Testing
	if (this->zEng->GetKeyListener()->IsPressed('Q'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);
			PlayerObject* pObject = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
			Equipment* eq = pObject->GetEquipmentPtr();

			Item* item = eq->GetWeapon();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

				this->zServerChannel->sendData(msg);
			}
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, false);
		}
	}

	if (this->zIsHuman)
	{
		if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INTERACT)))
		{
			if (!this->zKeyInfo.GetKeyState(KEY_INTERACT))
			{
				std::vector<Gui_Item_Data> collisionObjects = this->RayVsWorld();
				if (collisionObjects.size() > 0)
				{
					SendPickupItemMessage(collisionObjects[0].zID);
				}
				this->zGuiManager->ShowLootingGui(collisionObjects);
				this->zKeyInfo.SetKeyState(KEY_INTERACT, true);
			}
		}
		else
		{
			if (this->zKeyInfo.GetKeyState(KEY_INTERACT))
			{
				this->zGuiManager->HideLootingGui();
				this->zKeyInfo.SetKeyState(KEY_INTERACT, false);
			}

		}
		if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INVENTORY)))
		{
			if (!this->zKeyInfo.GetKeyState(KEY_INVENTORY))
			{
				this->zKeyInfo.SetKeyState(KEY_INVENTORY, true);
				this->zGuiManager->ToggleInventoryGui();
			}
		}
		else
		{
			if (this->zKeyInfo.GetKeyState(KEY_INVENTORY))
				this->zKeyInfo.SetKeyState(KEY_INVENTORY, false);
		}

		/*if (this->zEng->GetKeyListener()->IsClicked(2))
		{
			if (!this->zKeyInfo.GetKeyState(MOUSE_RIGHT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_RIGHT_PRESS, true);
				this->zGuiManager->ShowCircularItemGui();
			}
		}
		else
		{
			if (this->zKeyInfo.GetKeyState(MOUSE_RIGHT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_RIGHT_PRESS, false);
				this->zGuiManager->HideCircularItemGui();
			}
		}*/

		if (this->zEng->GetKeyListener()->IsClicked(1))
		{
			if (!this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, true);
				PlayerObject* player = this->zObjectManager->GetPlayerObject(index);

				Equipment* eq = player->GetEquipmentPtr();

				Weapon* weapon = eq->GetWeapon();

				if (!weapon)
				{
					this->DisplayMessageToClient("No Weapon is Equipped");
				}
				else
				{
					std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_WEAPON_USE, weapon->GetID());
					this->zServerChannel->sendData(msg);
				}
			}
		}
		else
		{
			if (this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, false);
		}
	}

	if (this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_MENU)))
	{
		this->zGuiManager->ToggleIngameMenu();
		std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_CONNECTION_CLOSED, this->zID);
		this->zServerChannel->sendData(msg);
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
			int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_PLAYER, msgArray[0]);
			this->UpdatePlayerObjects(msgArray, id);
		}
		//Animal
		else if(strcmp(key, M_UPDATE_ANIMAL.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_ANIMAL, msgArray[0]);
			this->UpdateAnimalObjects(msgArray, id);
		}
		//Static Object
		else if(strcmp(key, M_UPDATE_STATIC_OBJECT.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_STATIC_OBJECT, msgArray[0]);
			this->UpdateStaticObjects(msgArray, id);
		}
		//Static Object
		else if(strcmp(key, M_NEW_STATIC_OBJECT.c_str()) == 0)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_STATIC_OBJECT, msgArray[0]);
			this->AddNewStaticObject(msgArray, id);
		}
		//Static Object
		else if(strcmp(key, M_REMOVE_STATIC_OBJECT.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_STATIC_OBJECT, msgArray[0]);
			this->RemoveStaticObject(id);
		}
		//Dynamic Object
		else if(strcmp(key, M_UPDATE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_DYNAMIC_OBJECT, msgArray[0]);
			this->UpdateDynamicObjects(msgArray, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_NEW_DYNAMIC_OBJECT.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_NEW_DYNAMIC_OBJECT, msgArray[0]);
			this->AddNewDynamicObject(msgArray, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_REMOVE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_DYNAMIC_OBJECT, msgArray[0]);
			this->RemoveDynamicObject(id);
		}
		//Player
		else if(strcmp(key, M_NEW_PLAYER.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_NEW_PLAYER, msgArray[0]);
			this->AddNewPlayerObject(msgArray, id);
		}
		//Animal
		else if(strcmp(key, M_NEW_ANIMAL.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_NEW_ANIMAL, msgArray[0]);
			this->AddNewAnimalObject(msgArray, id);
		}
		//Animal
		else if(strcmp(key, M_REMOVE_ANIMAL.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_ANIMAL, msgArray[0]);
			this->RemoveAnimalObject(id);
		}
		//Player
		else if(strcmp(key, M_REMOVE_PLAYER.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_PLAYER, msgArray[0]);
			this->RemovePlayerObject(id);
		}
		else if(strcmp(key, M_EQUIP_ITEM.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_EQUIP_ITEM, msgArray[0]);
			int slot = -1;
			if (msgArray.size() > 1)
			{
				slot = this->zMsgHandler.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
			}
			else
			{
				MaloW::Debug("Forgot Slot Type in Equip Item");
			}
			this->HandleEquipItem(id, slot);
		}
		else if(strcmp(key, M_REMOVE_EQUIPMENT.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_EQUIPMENT, msgArray[0]);
			int slot = -1;
			if (msgArray.size() > 1)
			{
				slot = this->zMsgHandler.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
			}
			else
			{
				MaloW::Debug("Forgot Slot Type in Remove Equipment");
			}
			this->HandleRemoveEquipment(id, slot);
		}
		else if(strcmp(key, M_UNEQUIP_ITEM.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
			int slot = -1;
			if (msgArray.size() > 1)
			{
				slot = this->zMsgHandler.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
			}
			else
			{
				MaloW::Debug("Forgot Slot Type in UnEquip Item");
			}
			this->HandleUnEquipItem(id, slot);
		}
		else if(strcmp(key, M_ADD_INVENTORY_ITEM.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_ADD_INVENTORY_ITEM, msgArray[0]);
			this->HandleAddInventoryItem(msgArray, id);
		}
		else if(strcmp(key, M_REMOVE_INVENTORY_ITEM.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_INVENTORY_ITEM, msgArray[0]);
			this->HandleRemoveInventoryItem(id);
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
		else if(strcmp(key, M_ERROR_MESSAGE.c_str()) == 0)
		{
			std::string error_Message = this->zMsgHandler.ConvertStringToSubstring(M_ERROR_MESSAGE, msgArray[0]);
			DisplayMessageToClient(error_Message);
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

void Client::CloseConnection(const std::string& reason)
{
	MaloW::Debug("Client Shutdown: " + reason);
	//Todo Skriv ut vilket reason som gavs
	this->zServerChannel->Close();
	this->Close();
}

std::vector<Gui_Item_Data> Client::RayVsWorld()
{
	Vector3 origin = this->zEng->GetCamera()->GetPosition();
	Vector3 camForward = this->zEng->GetCamera()->GetForward();

	CollisionData data;
	std::vector<Gui_Item_Data> Collisions;
	//Static objects
	std::vector<StaticObject*> staticObjects = this->zObjectManager->GetStaticObjects();
	for(auto it = staticObjects.begin(); it < staticObjects.end(); it++)
	{
		iMesh* mesh = (*it)->GetMesh();
		if (!mesh)
		{
			MaloW::Debug("ERROR: Mesh is Null in RayVsWorld function");
			continue;
		}
		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMesh(origin, camForward, mesh);

		if (data.collision &&  data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			Gui_Item_Data gui_Data = Gui_Item_Data((*it)->GetID(), (*it)->GetName(), (*it)->GetIconPath(), (*it)->GetDescription());
			Collisions.push_back(gui_Data);
		}
	}
	//Dynamic objects
	std::vector<DynamicObject*> dynamicObjects = this->zObjectManager->GetDynamicObjects();
	for(auto it = dynamicObjects.begin(); it < dynamicObjects.end(); it++)
	{
		iMesh* mesh = (*it)->GetMesh();
		if (!mesh)
		{
			MaloW::Debug("ERROR: Mesh is Null in RayVsWorld function");
			continue;
		}
		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMesh(origin, camForward, mesh);

		if (data.collision &&  data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			Gui_Item_Data gui_Data = Gui_Item_Data((*it)->GetID(), (*it)->GetName(), (*it)->GetIconPath(), (*it)->GetDescription());
			Collisions.push_back(gui_Data);
		}
	}

	return Collisions;
}

bool Client::CheckCollision()
{
	int position = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, this->zID);

	if (position == -1)
		return false;

	CollisionData data;

	iMesh* playerMesh = this->zObjectManager->GetPlayerObject(position)->GetMesh();

	if (!playerMesh)
		return false;

	std::vector<iMesh*> mapObjects = this->zObjectManager->GetMapObjects();

	for (auto it_Map = mapObjects.begin(); it_Map < mapObjects.end(); it_Map++)
	{
		data = this->zEng->GetPhysicsEngine()->GetCollisionMeshMesh(playerMesh, (*it_Map));
		if (data.collision)
			return true;
	}

	return false;
}

bool Client::GetCursorVisibility()
{
	return this->zShowCursor;
}

void Client::DisplayMessageToClient(const std::string& msg)
{
	MaloW::Debug(msg);
}