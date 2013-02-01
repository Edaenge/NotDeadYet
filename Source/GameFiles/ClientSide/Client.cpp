#include "GameFiles/ClientSide/Client.h"
#include "Graphics.h"
#include "Network/NetworkPacket.h"
#include "../ClientServerMessages.h"
#include "ClientServerMessages.h"
#include "../WorldFiles/EntityList.h"
#include "../DebugMessages.h"

using namespace MaloW;

//Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;
// 50 updates per sec
static const float UPDATE_DELAY = 0.020f;
static const float MAX_DISTANCE_TO_OBJECT = 3.0f;

Client::Client()
{
	Messages::ClearDebug();

	this->zID = 0;
	this->zIP = "";
	this->zPort = 0;
	this->zIsHuman = true;
	this->zRunning = true;
	this->zCreated = false;
	this->zGameStarted = false;
	this->zShowCursor = false;
	this->zFrameTime = 0.0f;
	this->zTimeSinceLastPing = 0.0f;
	this->zMeshID = "Media/Models/scale.obj";
	this->zSendUpdateDelayTimer = 0.0f;

	this->zEng = NULL;
	this->zGuiManager = NULL;
	this->zObjectManager = NULL;
	this->zServerChannel = NULL;
	this->zPlayerInventory = NULL;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	
	this->zMsgHandler = NetworkMessageConverter();

	this->zWorld = 0;
	this->zWorldRenderer = 0;
	this->zAnchor = 0;
	this->zCrossHair = 0;
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

	SAFE_DELETE(this->zGuiManager);
	SAFE_DELETE(this->zObjectManager);
	SAFE_DELETE(this->zServerChannel);
	SAFE_DELETE(this->zPlayerInventory);

	SAFE_DELETE(this->zWorldRenderer);
	SAFE_DELETE(this->zWorld);
	SAFE_DELETE(this->zCrossHair);
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

	//Anchors with the world to decide what to render.
	if(zWorld && zAnchor)
	{
		Vector2 cameraPos = this->zEng->GetCamera()->GetPosition().GetXZ();

		this->zAnchor->position = cameraPos;
		this->zEng->SetSceneAmbientLight(this->zWorld->GetAmbientAtWorldPos(cameraPos));

		this->zAnchor->radius = this->zEng->GetEngineParameters().FarClip;
	}
	if(zWorld)
		this->zWorld->Update();

	return this->zDeltaTime;
}

void Client::InitGraphics()
{
	this->zEng->CreateSkyBox("Media/skymap.dds");
	this->zEng->GetCamera()->SetPosition( Vector3(54, 20, 44) );
	this->zEng->GetCamera()->LookAt( Vector3(50, 0, 50) );
	
	LoadEntList("Entities.txt");

	if ( zWorld ) delete zWorld, zWorld=0;
	this->zWorld = new World(this, "3x3.map");
	this->zWorldRenderer = new WorldRenderer(zWorld, GetGraphics());

	this->zAnchor = this->zWorld->CreateAnchor();
	this->zAnchor->position = Vector2(54, 44);
	this->zAnchor->radius = this->zEng->GetEngineParameters().FarClip;

	this->zWorld->Update();

	int windowWidth = GetGraphics()->GetEngineParameters().WindowWidth;
	int windowHeight = GetGraphics()->GetEngineParameters().WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;
	float length = ((25.0f / 1024.0f) * dx);
	float x = offSet + (0.5f * dx) - length * 0.5f;
	float y = (windowHeight / 2.0f) - length * 0.5f;

	this->zCrossHair = this->zEng->CreateImage(Vector2(x, y), Vector2(length, length), "Media/Icons/cross.png");

	std::string path = "Media/Models/";
	const char* object[] = {
		"Media/Models/ArmyRation_v01.obj", 
		"Media/Models/Arrow_v01.obj",
		"Media/Models/Bigleaf_01.ani", 
		"Media/Models/Bow_v01.obj",
		"Media/Models/BranchesItem_01_v01.obj", 
		"Media/Models/Bush_01.ani",
		"Media/Models/Campfire_01_v01.obj",
		"Media/Models/Fern_02.ani",
		"Media/Models/GrassPlant_01.ani",
		"Media/Models/Machete_v01.obj", 
		"Media/Models/Pocketknife_v02.obj", 
		"Media/Models/Stone_02_v01.obj",
		"Media/Models/Stone_01_v02.obj",
		"Media/Models/StoneItem_01_v01.obj",
		"Media/Models/Tree_01.ani",
		"Media/Models/WaterGrass_02.ani",
		"Media/Models/Veins_01_v03_r.obj"};

	this->zEng->PreLoadResources(17, object);
	this->zEng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png" ,"Media/LoadingScreen/LoadingScreenPB.png");

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

		if(this->zCreated && this->zGameStarted)
		{
			this->zSendUpdateDelayTimer += this->zDeltaTime;
			this->zTimeSinceLastPing += this->zDeltaTime;

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

			/*if (this->zTimeSinceLastPing > TIMEOUT_VALUE * 2.0f)
			{
				this->CloseConnection("Timeout");
			}
			else if (this->zTimeSinceLastPing > TIMEOUT_VALUE)
			{
				MaloW::Debug("Timeout From Server!");
				//Print a Timeout Message to Client
			}*/
		}

		if (this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_MENU)))
		{
			this->zGuiManager->ToggleIngameMenu();
			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_CONNECTION_CLOSED, (float)this->zID);
			this->zServerChannel->sendData(msg);
			this->CloseConnection("Escape was pressed");
		}

		Sleep(5);
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

void Client::SendAck(unsigned int IM_ID)
{
	std::string msg;
	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ACKNOWLEDGE, (float)IM_ID);

	this->zServerChannel->sendData(msg);
}

void Client::UpdateCameraPos()
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, this->zID);
	if (index != -1)
	{
		Vector3 position = this->zObjectManager->GetPlayerObject(index)->GetPosition();

		position.y += 2.0f;
		this->zEng->GetCamera()->SetPosition(position);
	}

	//Rotate Mesh
	Vector3 meshDir = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 camDir = this->zEng->GetCamera()->GetForward();
	Vector3 around;
	float angle;

	camDir.y = 0;
	camDir.Normalize();
	meshDir.Normalize();
	
	around = Vector3(0,1,0);

	angle = acos(camDir.GetDotProduct(meshDir));

	if(camDir.x > 0.0f)
		angle *= -1;

	iMesh* playerMesh = this->zObjectManager->SearchAndGetPlayerObject(this->zID)->GetMesh();

	playerMesh->ResetRotation();
	playerMesh->RotateAxis(around, angle);}

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
			msg = this->zMsgHandler.Convert(MESSAGE_TYPE_KEY_DOWN, (float)ID);

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
			msg = this->zMsgHandler.Convert(MESSAGE_TYPE_KEY_UP, (float)ID);

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

void Client::SendUseItemMessage(const long ID)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
	this->zServerChannel->sendData(msg);
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
						SendUseItemMessage(item->GetID());
					}
				}
				else if (msd.zAction == DROP)
				{
					this->SendDropItemMessage(msd.zID);
				}
				else if (msd.zAction == UNEQUIP)
				{
					this->SendUnEquipItem(msd.zID, EQUIPMENT_SLOT_MELEE_WEAPON);
				}
			}
		}
	}

	//UnEquip Melee Weapon
	if (this->zEng->GetKeyListener()->IsPressed('Q'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);
			PlayerObject* pObject = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
			Equipment* eq = pObject->GetEquipmentPtr();

			Item* item = eq->GetMeleeWeapon();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_MELEE_WEAPON);

				this->zServerChannel->sendData(msg);
			}
		}
	}
	//UnEquip Ranged Weapon
	if (this->zEng->GetKeyListener()->IsPressed('R'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);
			PlayerObject* pObject = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
			Equipment* eq = pObject->GetEquipmentPtr();

			Item* item = eq->GetRangedWeapon();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_RANGED_WEAPON);

				this->zServerChannel->sendData(msg);
			}
		}
	}
	//UnEquip Projectiles
	else if (this->zEng->GetKeyListener()->IsPressed('E'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);
			PlayerObject* pObject = this->zObjectManager->SearchAndGetPlayerObject(this->zID);
			Equipment* eq = pObject->GetEquipmentPtr();

			Item* item = eq->GetProjectile();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_AMMO);

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
				std::vector<Looting_Data> collisionObjects = this->RayVsWorld();
				std::vector<Gui_Item_Data> gui_Item_Datas;
				if (collisionObjects.size() > 0)
				{
					if (collisionObjects[0].type == OBJECT_TYPE_DYNAMIC_OBJECT || collisionObjects[0].type == OBJECT_TYPE_STATIC_OBJECT)
					{
						this->SendPickupItemMessage(collisionObjects[0].gid.zID);
					}
					else if (collisionObjects[0].type == OBJECT_TYPE_DEAD_PLAYER)
					{
						this->SendLootItemMessage(collisionObjects[0].owner, collisionObjects[0].gid.zID, collisionObjects[0].gid.zType);
					}
					for (auto x = collisionObjects.begin(); x < collisionObjects.end(); x++)
					{
						gui_Item_Datas.push_back((*x).gid);
					}
				}
				this->zGuiManager->ShowLootingGui(gui_Item_Datas);
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
		if (!this->zShowCursor)
		{
			if (this->zEng->GetKeyListener()->IsClicked(1))
			{
				if (!this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
				{
					this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, true);
					PlayerObject* player = this->zObjectManager->GetPlayerObject(index);

					Equipment* eq = player->GetEquipmentPtr();
					Weapon* weapon = eq->GetRangedWeapon();

					if (!weapon)
					{
						this->DisplayMessageToClient("No Weapon is Equipped");
					}
					else
					{
						std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_WEAPON_USE, (float)weapon->GetID());
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
		this->HandleWeaponEquips();
	}
	this->HandleDebugInfo();
}
//use to equip weapon with keyboard
void Client::HandleWeaponEquips()
{
	if (!this->zPlayerInventory)
		return;

	//Equip Bow
	if (this->zEng->GetKeyListener()->IsPressed('1'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_EQUIP))
		{
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_RANGED_BOW);
			if (item)
			{
				SendUseItemMessage(item->GetID());
			}

			this->zKeyInfo.SetKeyState(KEY_EQUIP, true);
		}
	}
	//Equip Arrow
	else if (this->zEng->GetKeyListener()->IsPressed('2'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_EQUIP))
		{
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_PROJECTILE_ARROW);
			if (item)
			{
				SendUseItemMessage(item->GetID());
			}

			this->zKeyInfo.SetKeyState(KEY_EQUIP, true);
		}
	}
	//Equip Rock
	else if (this->zEng->GetKeyListener()->IsPressed('3'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_EQUIP))
		{
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_RANGED_ROCK);
			if (item)
			{
				SendUseItemMessage(item->GetID());
			}

			this->zKeyInfo.SetKeyState(KEY_EQUIP, true);
		}
	}
	//Equip Axe
	else if (this->zEng->GetKeyListener()->IsPressed('4'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_EQUIP))
		{
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_MELEE_AXE);
			if (item)
			{
				SendUseItemMessage(item->GetID());
			}

			this->zKeyInfo.SetKeyState(KEY_EQUIP, true);
		}
	}
	//Equip Pocket Knife
	else if (this->zEng->GetKeyListener()->IsPressed('5'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_EQUIP))
		{
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE);
			if (item)
			{
				SendUseItemMessage(item->GetID());
			}

			this->zKeyInfo.SetKeyState(KEY_EQUIP, true);
		}
	}
	else if(this->zEng->GetKeyListener()->IsPressed('T'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			if(zEng->GetEngineParameters().WindowWidth != 1024)
			{
				zEng->ResizeGraphicsEngine(1024, 768);
				this->zGuiManager->Resize(1024, 768);
			}
			else
			{
				zEng->ResizeGraphicsEngine(1800, 1000);
				this->zGuiManager->Resize(1800, 1000);
			}

			this->zKeyInfo.SetKeyState(KEY_TEST, true);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_EQUIP))
		{
			this->zKeyInfo.SetKeyState(KEY_EQUIP, false);
		}
	}
}

void Client::HandleDebugInfo()
{
	//Terrain Height debug
	if (this->zEng->GetKeyListener()->IsPressed(VK_F1))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Terrain problem at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Terrain Texture debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F2))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Terrain Texture problem at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Terrain AI Grid debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F3))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Terrain AI grid problem at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Object debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F4))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Object problem at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	else if (this->zEng->GetKeyListener()->IsPressed(VK_DELETE))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			DebugMsg::ClearDebug();

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, false);
		}
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
	
	if(msgArray.size() > 0)
	{
		char key[1024];
		sscanf_s(msgArray[0].c_str(), "%s ", &key, sizeof(key));

		//Checks if the message has a 'important' tag.
		if(strcmp(key, M_IMPORTANT_MESSAGE.c_str()) == 0)
		{
			unsigned int IM_ID = this->zMsgHandler.ConvertStringToInt(M_IMPORTANT_MESSAGE, msgArray[0]);
			SendAck(IM_ID);

			msgArray.erase(msgArray.begin());
			sscanf_s(msgArray[0].c_str(), "%s ", &key, sizeof(key));
		}

		//Checks what type of message was sent
		if(strcmp(key, M_PING.c_str()) == 0)
		{
			this->Ping();
		}
		//Player
		else if(strcmp(key, M_UPDATE_PLAYER.c_str()) == 0)
		{ 
			long id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_PLAYER, msgArray[0]);
			this->UpdatePlayerObjects(msgArray, id);
		}
		//Animal
		else if(strcmp(key, M_UPDATE_ANIMAL.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_ANIMAL, msgArray[0]);
			this->UpdateAnimalObjects(msgArray, id);
		}
		//Static Object
		else if(strcmp(key, M_UPDATE_STATIC_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_STATIC_OBJECT, msgArray[0]);
			this->UpdateStaticObjects(msgArray, id);
		}
		//Static Object
		else if(strcmp(key, M_NEW_STATIC_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_NEW_STATIC_OBJECT, msgArray[0]);
			this->AddNewStaticObject(msgArray, id);
		}
		//Static Object
		else if(strcmp(key, M_REMOVE_STATIC_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_STATIC_OBJECT, msgArray[0]);
			this->RemoveStaticObject(id);
		}
		//Dynamic Object
		else if(strcmp(key, M_UPDATE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_DYNAMIC_OBJECT, msgArray[0]);
			this->UpdateDynamicObjects(msgArray, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_NEW_DYNAMIC_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_NEW_DYNAMIC_OBJECT, msgArray[0]);
			this->AddNewDynamicObject(msgArray, id);
		}
		//Dynamic Object
		else if(strcmp(key, M_REMOVE_DYNAMIC_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_DYNAMIC_OBJECT, msgArray[0]);
			this->RemoveDynamicObject(id);
		}
		//Player
		else if(strcmp(key, M_NEW_PLAYER.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_NEW_PLAYER, msgArray[0]);
			this->AddNewPlayerObject(msgArray, id);
		}
		//Animal
		else if(strcmp(key, M_NEW_ANIMAL.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_NEW_ANIMAL, msgArray[0]);
			this->AddNewAnimalObject(msgArray, id);
		}
		//Animal
		else if(strcmp(key, M_REMOVE_ANIMAL.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_ANIMAL, msgArray[0]);
			this->RemoveAnimalObject(id);
		}
		//Player
		else if(strcmp(key, M_REMOVE_PLAYER.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_PLAYER, msgArray[0]);
			this->RemovePlayerObject(id);
		}
		else if(strcmp(key, M_EQUIP_ITEM.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_EQUIP_ITEM, msgArray[0]);
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
		else if(strcmp(key, M_ITEM_USE.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
			this->HandleUseItem(id);
		}
		else if(strcmp(key, M_REMOVE_EQUIPMENT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_EQUIPMENT, msgArray[0]);
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
			long id = this->zMsgHandler.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
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
			long id = this->zMsgHandler.ConvertStringToInt(M_ADD_INVENTORY_ITEM, msgArray[0]);
			this->HandleAddInventoryItem(msgArray, id);
		}
		else if(strcmp(key, M_REMOVE_INVENTORY_ITEM.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_INVENTORY_ITEM, msgArray[0]);
			this->HandleRemoveInventoryItem(id);
		}
		else if(strcmp(key, M_WEAPON_USE.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);
			this->HandleWeaponUse(id);
		}
		else if(strcmp(key, M_ADD_DEAD_PLAYER_OBJECT.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_ADD_DEAD_PLAYER_OBJECT, msgArray[0]);

			this->AddNewDeadPlayerObject(msgArray, id);
		}
		else if(strcmp(key, M_DEAD_PLAYER_REMOVE_ITEM.c_str()) == 0)
		{
			long id = this->zMsgHandler.ConvertStringToInt(M_DEAD_PLAYER_REMOVE_ITEM, msgArray[0]);
			int type = -1;
			long itemID = -1;
			if (msgArray.size() > 2)
			{
				itemID = this->zMsgHandler.ConvertStringToInt(M_ITEM_ID, msgArray[1]);
				type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, msgArray[2]);

				this->HandeRemoveDeadPlayerItem(id, itemID, type);
			}
			else
			{
				MaloW::Debug("Msg array size is to short size: " + MaloW::convertNrToString((float)msgArray.size()) + " Expected size 3");
			}
		}
		else if(strcmp(key, M_DEAD_PLAYER.c_str()) == 0)
		{
			int id = this->zMsgHandler.ConvertStringToInt(M_DEAD_PLAYER, msgArray[0]);

			this->HandleDeadPlayerMessage(id);
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
		else if(strcmp(key, M_START_GAME.c_str()) == 0)
		{
			this->zGameStarted = true;
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[0] + " in HandleNetworkMessage");
			MaloW::Debug("C: " + msg);
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

std::vector<Looting_Data> Client::RayVsWorld()
{
	Vector3 origin = this->zEng->GetCamera()->GetPosition();
	Vector3 camForward = this->zEng->GetCamera()->GetForward();

	CollisionData data;
	std::vector<Looting_Data> Collisions;
	//Static objects
	std::vector<StaticObject*> staticObjects = this->zObjectManager->GetStaticObjects();
	iMesh* mesh = NULL;
	for(auto it = staticObjects.begin(); it < staticObjects.end(); it++)
	{
		mesh = (*it)->GetMesh();
		if (!mesh)
		{
			MaloW::Debug("ERROR: Mesh is Null in RayVsWorld function");
			continue;
		}
		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMeshBoundingOnly(origin, camForward, mesh);

		if (data.collision && data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			Looting_Data ld;
			Gui_Item_Data gui_Data = Gui_Item_Data((*it)->GetID(), (*it)->GetWeight(), (*it)->GetStackSize(), 
				(*it)->GetName(), (*it)->GetIconPath(), (*it)->GetDescription(), (*it)->GetType());

			ld.owner = gui_Data.zID;
			ld.gid = gui_Data;
			ld.type = OBJECT_TYPE_DYNAMIC_OBJECT;

			Collisions.push_back(ld);
		}
	}
	//Dynamic objects
	std::vector<DynamicObject*> dynamicObjects = this->zObjectManager->GetDynamicObjects();
	for(auto it = dynamicObjects.begin(); it < dynamicObjects.end(); it++)
	{
		mesh = (*it)->GetMesh();
		if (!mesh)
		{
			MaloW::Debug("ERROR: Mesh is Null in RayVsWorld function");
			continue;
		}
		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMeshBoundingOnly(origin, camForward, mesh);

		if (data.collision && data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			Looting_Data ld;
			Gui_Item_Data gui_Data = Gui_Item_Data((*it)->GetID(), (*it)->GetWeight(), (*it)->GetStackSize(), 
				(*it)->GetName(), (*it)->GetIconPath(), (*it)->GetDescription(), (*it)->GetType());

			ld.owner = gui_Data.zID;
			ld.gid = gui_Data;
			ld.type = OBJECT_TYPE_DYNAMIC_OBJECT;

			Collisions.push_back(ld);
		}
	}

	std::vector<DeadPlayerObject*> deadPlayerObjects = this->zObjectManager->GetDeadPlayerObjects();
	for (auto it = deadPlayerObjects.begin(); it < deadPlayerObjects.end(); it++)
	{
		mesh = (*it)->GetMesh();
		if (!mesh)
		{
			MaloW::Debug("ERROR: Mesh is Null in RayVsWorld function");
			continue;
		}

		data = this->zEng->GetPhysicsEngine()->GetCollisionRayMeshBoundingOnly(origin, camForward, mesh);

		if (data.collision && data.distance < MAX_DISTANCE_TO_OBJECT)
		{
			std::vector<Item*> items = (*it)->GetItems();
			for (auto x = items.begin(); x < items.end(); x++)
			{
				Looting_Data ld;
				Gui_Item_Data gui_Data = Gui_Item_Data((*x)->GetID(), (*x)->GetWeight(), (*x)->GetStackSize(), 
					(*x)->GetItemName(), (*x)->GetIconPath(), (*x)->GetItemDescription(), (*x)->GetItemType());

				ld.owner = (*it)->GetID();
				ld.gid = gui_Data;
				ld.type = OBJECT_TYPE_DEAD_PLAYER;

				Collisions.push_back(ld);
			}
			
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

void Client::onEvent(Event* e)
{
	if ( WorldLoadedEvent* WLE = dynamic_cast<WorldLoadedEvent*>(e) )
	{
		// Create Anchor
		zAnchor = WLE->world->CreateAnchor();
	}
	else if ( WorldDeletedEvent* WDE = dynamic_cast<WorldDeletedEvent*>(e) )
	{
		if ( zWorldRenderer ) delete zWorldRenderer, zWorldRenderer = 0;
		if ( zAnchor ) zWorld->DeleteAnchor( zAnchor );
		if ( zWorld ) zWorld = 0;
	}
	else if ( EntityRemovedEvent *ERE = dynamic_cast<EntityRemovedEvent*>(e) )
	{
		//zTargetedEntities.erase(ERE->entity);
	}
}