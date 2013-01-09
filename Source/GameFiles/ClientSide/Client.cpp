#include "GameFiles/ClientSide/Client.h"
#include "Graphics.h"
#include "Safe.h"
#include "Network/NetworkPacket.h"

using namespace MaloW;

//Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;
// 30 updates per sec
static const float UPDATE_DELAY = 0.0333f;
static const float MAX_DISTANCE_TO_OBJECT = 10.0f;

Client::Client()
{
	this->zID = 0;
	this->zIP = "";
	this->zPort = 0;
	this->zRunning = true;
	this->zCreated = false;
	this->zShowCursor = false;
	this->zFrameTime = 0.0f;
	this->zTimeSinceLastPing = 0.0f;
	this->zMeshID = "Media/scale.obj";
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

	//Used For Testing ATM
	if (this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_JUMP)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_JUMP))
		{
			if (this->zPlayerInventory->GetItems().size() > 0)
			{
 				int id = this->zPlayerInventory->GetItem(0)->GetID();
				this->SendDropItemMessage(id);
			}
		}
		this->zKeyInfo.SetKeyState(KEY_JUMP, true);
	}
	else
	{
		this->zKeyInfo.SetKeyState(KEY_JUMP, false);
	}
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
		this->zKeyInfo.SetKeyState(KEY_INVENTORY, false);
	}

	if (this->zEng->GetKeyListener()->IsClicked(2))
	{
		this->zGuiManager->ShowCircularItemGui();
	}
	else
	{
		this->zGuiManager->HideCircularItemGui();
	}

	if (this->zEng->GetKeyListener()->IsClicked(1))
	{
		PlayerObject* player = this->zObjectManager->GetPlayerObject(index);
		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(player->GetEquipmentPtr()->GetWeapon());
		if (!mWpn)
		{
			RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(player->GetEquipmentPtr()->GetWeapon());
			if (rWpn)
			{
			}
		}
		else
		{

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
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_PLAYER, msgArray[0]);
			this->RemovePlayerObject(id);
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

void Client::HandleAddInventoryItem(const std::vector<std::string>& msgArray, const unsigned int id)
{
	std::string itemName = "Unknown";
	std::string itemDescription = "<<<<<UNKNOWN DESCRIPTION>>>>>";
	std::string itemIconFilePath = "none";
	int itemWeight = 0;
	int itemType = -1;
	float weaponDamage = 0.0f;
	float weaponRange = 0.0f;
	float hunger = 0.0f;
	int maxUse = 0;
	int currUse = 0;
	char key[256];
	for (auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			itemName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			itemDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			itemIconFilePath = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			itemWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
		}
		else if(strcmp(key, M_WEAPON_DAMAGE.c_str()) == 0)
		{
			weaponDamage = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_DAMAGE, (*it));
		}
		else if(strcmp(key, M_WEAPON_RANGE.c_str()) == 0)
		{
			weaponRange = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_RANGE, (*it));
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, (*it));
		}
		else if(strcmp(key, M_CONTAINER_MAX.c_str()) == 0)
		{
			maxUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_MAX, (*it));
		}
		else if(strcmp(key, M_CONTAINER_CURRENT.c_str()) == 0)
		{
			currUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_CURRENT, (*it));
		}
	}
	if (itemType == -1)
	{
		MaloW::Debug("Wrong or no Item Type sent from server in Client::HandleAddInventoryItem ItemType: " + MaloW::convertNrToString(itemType));
		return;
	}
	//Todo add more data to item to identify type ex Bow/Axe/Pocket Knife
	Item* item = NULL;
	switch (itemType)
	{
	case ITEM_TYPE_FOOD_DEER_MEAT:
		item = new Food(id, itemWeight, itemName, itemType, itemDescription, hunger);
		break;
	case ITEM_TYPE_WEAPON_RANGED_BOW:
		item = new RangedWeapon(id, itemWeight, itemName, itemType, itemDescription, weaponDamage, weaponRange);
		break;
	case ITEM_TYPE_WEAPON_RANGED_ROCK:
		item = new RangedWeapon(id, itemWeight, itemName, itemType, itemDescription, weaponDamage, weaponRange);
		break;
	case ITEM_TYPE_WEAPON_MELEE_AXE:
		item = new MeleeWeapon(id, itemWeight, itemName, itemType, itemDescription, weaponDamage, weaponRange);
		break;
	case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
		item = new MeleeWeapon(id, itemWeight, itemName, itemType, itemDescription, weaponDamage, weaponRange);
		break;
	case ITEM_TYPE_CONTAINER_CANTEEN:
		item = new Container(id, itemWeight, itemName, itemType, itemDescription, maxUse, currUse);
		break;
	case ITEM_TYPE_GEAR_HEAD:
		item = new Gear(id, itemWeight, itemName, itemType, itemDescription);
		break;
	case ITEM_TYPE_GEAR_CHEST:
		item = new Gear(id, itemWeight, itemName, itemType, itemDescription);
		break;
	case ITEM_TYPE_GEAR_LEGS:
		item = new Gear(id, itemWeight, itemName, itemType, itemDescription);
		break;
	case ITEM_TYPE_GEAR_BOOTS:
		item = new Gear(id, itemWeight, itemName, itemType, itemDescription);
		break;
	default:
		break;
	}
	if (this->zPlayerInventory->AddItem(item))
	{
		Gui_Item_Data gid = Gui_Item_Data(id, itemName, itemDescription, itemIconFilePath);
		//this->zGuiManager->AddInventoryItemToGui(gid);

		MaloW::Debug("Added Image ID: " + MaloW::convertNrToString(id));
		TempImage temp;
		int pos = images.size();
		float width = this->zEng->GetEngineParameters()->windowWidth * 0.1428f;
		float height = this->zEng->GetEngineParameters()->windowHeight * 0.1428f;
		int y = (int)(pos * 0.1428f);
		temp.image = this->zEng->CreateImage(Vector2((pos  - y)* width, y * height + 50), Vector2(width, width), itemIconFilePath.c_str());
		temp.id = id;

		this->images.push_back(temp);
	}
	else
	{
		if (item)
		{
			delete item;
			item = NULL;
		}
	}
}

void Client::HandleUseItem(const int id)
{
	int index = this->zPlayerInventory->Search(id);

	Item* item = this->zPlayerInventory->GetItem(index);

	if(!item)
		return;
	
	if(item->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN)
	{
		item->Use();
	}
	else if(item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW || item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
	{
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);

		Equipment* eq = player->GetEquipmentPtr();

		RangedWeapon* rwp = dynamic_cast<RangedWeapon*>(item);

		if (rwp)
			eq->EquipWeapon(rwp);
	}
	else if(item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE || item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
	{
		PlayerObject* player = this->zObjectManager->SearchAndGetPlayerObject(this->zID);

		Equipment* eq = player->GetEquipmentPtr();

		MeleeWeapon* rwp = dynamic_cast<MeleeWeapon*>(item);

		if (rwp)
			eq->EquipWeapon(rwp);
	}
	else if(item->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT)
	{
		item->Use();
	}
}

void Client::HandleRemoveInventoryItem(const int id)
{
	int index = this->zPlayerInventory->Search(id);
	if(this->zPlayerInventory->RemoveItem(index))
		MaloW::Debug("Item Removed on Client");

	for (unsigned int i = 0; i < images.size(); i++)
	{
		if (images[i].id == id)
		{
			MaloW::Debug("Removed Image ID: " + MaloW::convertNrToString(id));
			this->zEng->DeleteImage(images[i].image);
			images.erase(images.begin() + i);
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

bool Client::AddNewPlayerObject(const std::vector<std::string>& msgArray, const int id)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, id);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Object. Id already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	PlayerObject* playerObject = new PlayerObject(id);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
			playerObject->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			playerObject->SetState(state);
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewPlayerObject");
		}
	}
	if (!this->zCreated)
	{
		if (id == this->zID)
		{
			this->zCreated = true;
		}
	}
	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	playerObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(playerObject);

	return true;
}

bool Client::AddNewAnimalObject(const std::vector<std::string>& msgArray, const int id)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, id);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Object. It already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	AnimalObject* animalObject = new AnimalObject(id);
	
	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
			animalObject->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			animalObject->SetState(state);
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewAnimalObject");
		}
	}
	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	animalObject->SetStaticMesh(mesh);

	
	this->zObjectManager->AddObject(animalObject);

	return true;
}

bool Client::AddNewStaticObject(const std::vector<std::string>& msgArray, const int id)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, id);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Object. It already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	StaticObject* staticObject = new StaticObject(id);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			staticObject->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			staticObject->SetWeight(weight);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			staticObject->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			staticObject->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			staticObject->SetIconPath(path);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewStaticObject");
		}
	}

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	staticObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(staticObject);

	return true;
}

bool Client::AddNewDynamicObject(const std::vector<std::string>& msgArray, const int id)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, id);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Object. It already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);

	std::string filename = "";
	DynamicObject* dynamicObject = new DynamicObject(id);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				float y = terrain->GetYPositionAt(position.x, position.z);
				if (position.y < y)
					position.y = y;
			}
			dynamicObject->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			dynamicObject->SetState(state);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			dynamicObject->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			dynamicObject->SetWeight(weight);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			dynamicObject->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			dynamicObject->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			dynamicObject->SetIconPath(path);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewDynamicObject");
		}
	}
	
	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	dynamicObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(dynamicObject);

	return true;
}

bool Client::UpdatePlayerObjects(const std::vector<std::string>& msgArray, const int id)
{
	//Get ID and Position Depending on type
	if (id == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, id);
	
	//Check if object was found
	if(index == -1)
		return false;

	PlayerObject* PlayerObjectPointer = this->zObjectManager->GetPlayerObject(index);

	if (!PlayerObjectPointer)
		return false;

	Vector3 position;
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;
		
	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			//iTerrain* terrain = this->zObjectManager->GetTerrain();
			//if (terrain)
			//{
			//	position.y = terrain->GetYPositionAt(position.x, position.z);
			//}
			PlayerObjectPointer->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			PlayerObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			PlayerObjectPointer->SetState(state);
		}
		else if(strcmp(key, M_HEALTH.c_str()) == 0)
		{
			float health = this->zMsgHandler.ConvertStringToFloat(M_HEALTH, (*it));
			PlayerObjectPointer->SetHealth(health);
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			float hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, (*it));
			PlayerObjectPointer->SetHunger(hunger);
		}
		else if(strcmp(key, M_HYDRATION.c_str()) == 0)
		{
			float hydration = this->zMsgHandler.ConvertStringToFloat(M_HYDRATION, (*it));
			PlayerObjectPointer->SetHydration(hydration);
		}
		else if(strcmp(key, M_STAMINA.c_str()) == 0)
		{
			float stamina = this->zMsgHandler.ConvertStringToFloat(M_STAMINA, (*it));
			PlayerObjectPointer->SetStamina(stamina);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdatePlayerObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), PlayerObjectPointer->GetPosition());
		float scale = PlayerObjectPointer->GetScale().y;
		Vector4 quat = PlayerObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (PlayerObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(PlayerObjectPointer->GetMesh());
		}
		PlayerObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::UpdateStaticObjects(const std::vector<std::string>& msgArray, const int id)
{
	//Get ID and Position Depending on type
	if (id == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, id);
	
	//Check if object was found
	if(index == -1)
		return false;

	StaticObject* StaticObjectPointer = this->zObjectManager->GetStaticObject(index);
	if (!StaticObjectPointer)
		return false;

	Vector3 position;
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;
	
	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			//iTerrain* terrain = this->zObjectManager->GetTerrain();
			//if (terrain)
			//{
			//	position.y = terrain->GetYPositionAt(position.x, position.z);
			//}
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			StaticObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			StaticObjectPointer->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			StaticObjectPointer->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			StaticObjectPointer->SetIconPath(path);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			StaticObjectPointer->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			StaticObjectPointer->SetWeight(weight);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdateStaticObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), StaticObjectPointer->GetPosition());
		float scale = StaticObjectPointer->GetScale().y;
		Vector4 quat = StaticObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (StaticObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(StaticObjectPointer->GetMesh());
		}
		StaticObjectPointer->SetStaticMesh(mesh);
	}
	return true;
}

bool Client::UpdateAnimalObjects(const std::vector<std::string>& msgArray, const int id)
{
	//Get ID and Position Depending on type
	if (id == -1)
		return false;
	
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, id);
	//Check if object was found
	if(index == -1)
		return false;	

	AnimalObject* AnimalObjectPointer = this->zObjectManager->GetAnimalObject(index);

	if (!AnimalObjectPointer)
		return false;

	Vector3 position;
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
			AnimalObjectPointer->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			AnimalObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			AnimalObjectPointer->SetState(state);
		}
		else if(strcmp(key, M_HEALTH.c_str()) == 0)
		{
			float health = this->zMsgHandler.ConvertStringToFloat(M_HEALTH, (*it));
			AnimalObjectPointer->SetHealth(health);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdateAnimalObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), AnimalObjectPointer->GetPosition());
		float scale = AnimalObjectPointer->GetScale().y;
		Vector4 quat = AnimalObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (AnimalObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(AnimalObjectPointer->GetMesh());
		}
		AnimalObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::UpdateDynamicObjects(const std::vector<std::string>& msgArray, const int id)
{
	//Get ID and Position Depending on type
	if (id == -1)
		return false;
		
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, id);
	
	//Check if object was found
	if(index == -1)
		return false;

	DynamicObject* DynamicObjectPointer = this->zObjectManager->GetDynamicObject(index);

	if (DynamicObjectPointer == NULL)
		return false;

	Vector3 position;
	
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			DynamicObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			DynamicObjectPointer->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			DynamicObjectPointer->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			DynamicObjectPointer->SetIconPath(path);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			DynamicObjectPointer->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			DynamicObjectPointer->SetWeight(weight);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdateDynamicObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), DynamicObjectPointer->GetPosition());
		float scale = DynamicObjectPointer->GetScale().y;
		Vector4 quat = DynamicObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (DynamicObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(DynamicObjectPointer->GetMesh());
		}
		DynamicObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::RemovePlayerObject(const int id)
{
	if (id == -1)
		return false;
		
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, id);

	//Check if object was found in the array
	if(index == -1)
		return false;

	if(this->zID == id)
	{
		this->CloseConnection("Unknown reason possible Kicked");
	}

	iMesh* mesh = this->zObjectManager->GetPlayerObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_PLAYER, index))
	{
		MaloW::Debug("Failed To Remove Player with id: " + MaloW::convertNrToString(id));
	}
	return true;
}

bool Client::RemoveAnimalObject(const int id)
{
	if (id == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, id);

	//Check if object was found in the array
	if(index == -1)
		return false;

	iMesh* mesh = this->zObjectManager->GetAnimalObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_ANIMAL, index))
	{
		MaloW::Debug("Failed To Remove Animal with id: " + MaloW::convertNrToString(id));
	}

	return true;
}

bool Client::RemoveStaticObject(const int id)
{
	if (id == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, id);

	//Check if object was found in the array
	if(index == -1)
		return false;
	
	iMesh* mesh = this->zObjectManager->GetStaticObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_STATIC_OBJECT, index))
	{
		MaloW::Debug("Failed To Remove Static Object with id: " + MaloW::convertNrToString(id));
		return false;
	}
	MaloW::Debug("Removed Static Object, Number of Objects remaining = " + MaloW::convertNrToString(this->zObjectManager->GetStaticObjects().size()));

	return true;
}

bool Client::RemoveDynamicObject(const int id)
{
	if (id == -1)
		return false;

	int pos = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, id);

	//Check if client was found in the array
	if(pos == -1)
		return false;

	iMesh* mesh = this->zObjectManager->GetDynamicObject(pos)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_DYNAMIC_OBJECT, pos))
	{
		MaloW::Debug("Failed To Remove Player with id: " + MaloW::convertNrToString(id));
	}

	return true;
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
		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMesh(origin, camForward, mesh);

		if (data.collision &&  data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			Gui_Item_Data gui_Data = Gui_Item_Data((*it)->GetID(), (*it)->GetName(), "", (*it)->GetDescription());
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

void Client::SendPickupItemMessage(const int id)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_PICKUP_ITEM, id);

	this->zServerChannel->sendData(msg);
}

void Client::SendDropItemMessage(const int id)
{
	std::string msg;

	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_DROP_ITEM, id);

	this->zServerChannel->sendData(msg);
}

bool Client::GetCursorVisibility()
{
	return this->zShowCursor;
}

void Client::DisplayMessageToClient(const std::string& msg)
{
	MaloW::Debug(msg);
}