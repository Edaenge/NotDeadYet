#include "Client.h"
#include "Graphics.h"
#include <NetworkPacket.h>
#include <ClientServerMessages.h>
#include <ClientServerMessages.h>
#include <World/EntityList.h>
#include "DebugMessages.h"
#include <DisconnectedEvent.h>
#include "Sounds.h"
using namespace MaloW;

// Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;

// 50 updates per second
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
	this->zMeshID = "Media/Models/temp_guy.obj";
	this->zSendUpdateDelayTimer = 0.0f;

	this->zEng = NULL;
	this->zGuiManager = NULL;
	this->zObjectManager = NULL;
	this->zServerChannel = NULL;
	this->zPlayerInventory = NULL;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	
	this->zMsgHandler = NetworkMessageConverter();

	this->zWorld = NULL;
	this->zWorldRenderer = NULL;
	this->zAnchor = NULL;
	this->zCrossHair = NULL;
	SoundsInit();

	GetSounds()->LoadSoundIntoSystem("Media/Sound/Walk.wav", false);
	GetSounds()->LoadSoundIntoSystem("Media/Sound/Breath.wav", false);
}

void Client::Connect(const std::string &IPAddress, const unsigned int &port)
{
	this->zIP = IPAddress;
	this->zPort = port;
	this->zServerChannel = new ServerChannel(this, IPAddress, port);
	this->zServerChannel->Start();
}

Client::~Client()
{
	this->Close();
	this->WaitUntillDone();

	SAFE_DELETE(this->zGuiManager);
	SAFE_DELETE(this->zObjectManager);
	SAFE_DELETE(this->zServerChannel);
	SAFE_DELETE(this->zPlayerInventory);

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

	// Anchors with the world to decide what to render.
	if(zWorld)
	{
		if (zAnchor)
		{
			Vector2 cameraPos = this->zEng->GetCamera()->GetPosition().GetXZ();

			this->zAnchor->position = cameraPos;
			this->zEng->SetSceneAmbientLight(this->zWorld->GetAmbientAtWorldPos(cameraPos));

			this->zAnchor->radius = this->zEng->GetEngineParameters().FarClip;
		}

		this->zWorld->Update();
		if ( zWorldRenderer ) zWorldRenderer->Update();
	}		

	return this->zDeltaTime;
}

void Client::InitGraphics(const std::string& mapName)
{
	this->zEng->CreateSkyBox("Media/skymap.dds");

	LoadEntList("Entities.txt");

	if ( zWorld ) delete zWorld, zWorld=0;
	this->zWorld = new World(this, mapName);
	this->zWorldRenderer = new WorldRenderer(zWorld, this->zEng);

	float x = this->zWorld->GetWorldCenter().x;
	float z = this->zWorld->GetWorldCenter().y;

	Vector2 center = Vector2(x, z);

	this->zEng->GetCamera()->SetPosition( Vector3(center.x, 20, center.y) );
	this->zEng->GetCamera()->LookAt( Vector3(center.x, 0, center.y) );

	this->zAnchor = this->zWorld->CreateAnchor();
	this->zAnchor->position = center;
	this->zAnchor->radius = this->zEng->GetEngineParameters().FarClip;

	int windowWidth = GetGraphics()->GetEngineParameters().WindowWidth;
	int windowHeight = GetGraphics()->GetEngineParameters().WindowHeight;	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;
	float length = ((25.0f / 1024.0f) * dx);
	float xPos = offSet + (0.5f * dx) - length * 0.5f;
	float yPos = (windowHeight / 2.0f) - length * 0.5f;

	this->zCrossHair = this->zEng->CreateImage(Vector2(xPos, yPos), Vector2(length, length), "Media/Icons/cross.png");

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
		"Media/Models/Veins_01_v03_r.obj",
		"Media/Models/temp_guy.obj"};

	this->zEng->PreLoadResources(18, object);
	this->zEng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png" ,"Media/LoadingScreen/LoadingScreenPB.png");	//this->zEng->StartRendering();
}

void Client::Init()
{
	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);

	this->zEng = GetGraphics();

	this->zObjectManager = new ClientActorManager();
	this->zGuiManager = new GuiManager(this->zEng);
	this->zPlayerInventory = new Inventory();
}

void Client::Life()
{
	MaloW::Debug("Client Process Started");

	this->Init();

	while(this->zEng->IsRunning() && this->stayAlive)
	{
		this->Update();

		this->HandleKeyboardInput();
		if(this->zCreated)
		{
			this->zSendUpdateDelayTimer += this->zDeltaTime;
			this->zTimeSinceLastPing += this->zDeltaTime;

 			if(this->zSendUpdateDelayTimer >= UPDATE_DELAY)
 			{
				this->zSendUpdateDelayTimer = 0.0f;

				this->SendClientUpdate();
			}

			this->UpdateMeshRotation(); // Will manage the local model later.

			this->UpdateActors();
		}

		if (this->stayAlive)
		{
			this->ReadMessages();
		}

		if (this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_MENU)))
		{
			this->zGuiManager->ToggleIngameMenu();
			//this->CloseConnection("");
		}

		Sleep(5);
	}

	this->zRunning = false;
}

void Client::ReadMessages()
{
	static const unsigned int MAX_NR_OF_MESSAGES = 10000;

	int nrOfMessages = this->GetEventQueueSize();
	if (nrOfMessages == 0)
		return;

	int messages_To_Read = min(MAX_NR_OF_MESSAGES, nrOfMessages);
	for (int i = 0; i < messages_To_Read; i++)
	{
		if (MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			//Check if Client has received a Message
			if ( MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(ev) )
			{
				HandleNetworkMessage(np->GetMessage());
			}
			else if ( DisconnectedEvent* np = dynamic_cast<DisconnectedEvent*>(ev) )
			{
				CloseConnection("Unknown");
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
	Vector4 rot = Vector4(0, 0, 0, 0);

	Actor* player = this->zObjectManager->SearchAndGetActor(this->zID);
	
	if (player)
	{
		rot = player->GetRotation();
	}
	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_CLIENT_DATA);
	//msg += this->zMsgHandler.Convert(MESSAGE_TYPE_FRAME_TIME, this->zFrameTime);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_DIRECTION, dir);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_UP, up);
	msg += this->zMsgHandler.Convert(MESSAGE_TYPE_ROTATION, rot);
	
	this->zServerChannel->TrySend(msg);
}

void Client::SendAck(unsigned int IM_ID)
{
	std::string msg;
	msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ACKNOWLEDGE, (float)IM_ID);

	this->zServerChannel->Send(msg);
}

void Client::UpdateMeshRotation()
{
	Actor* player = this->zObjectManager->SearchAndGetActor(this->zID);
	if (!player)
	{
		return;
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

	iMesh* playerMesh = player->GetMesh();

	playerMesh->ResetRotation();
	playerMesh->RotateAxis(around, angle);
}

void Client::UpdateActors()
{
	this->zObjectManager->UpdateObjects(this->zDeltaTime, this->zID);
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

			this->zServerChannel->Send(msg);
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

			this->zServerChannel->Send(msg);
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

void Client::SendUseItemMessage(const unsigned int ID)
{
	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ITEM_USE, (float)ID);
	this->zServerChannel->Send(msg);
}

void Client::HandleKeyboardInput()
{
	if (this->zCreated && this->zGameStarted)
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
				if (msd.zAction == USE || msd.zAction == EQUIP)
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
					this->SendUnEquipItem(msd.zID, (msd.zType));
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
			

			Item* item = this->zPlayerInventory->GetMeleeWeapon();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_MELEE_WEAPON);

				this->zServerChannel->Send(msg);
			}
		}
	}
	//UnEquip Ranged Weapon
	if (this->zEng->GetKeyListener()->IsPressed('R'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);
			

			Item* item = this->zPlayerInventory->GetRangedWeapon();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_RANGED_WEAPON);

				this->zServerChannel->Send(msg);
			}
		}
	}
	//UnEquip Projectiles
	else if (this->zEng->GetKeyListener()->IsPressed('E'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);

			Item* item = this->zPlayerInventory->GetProjectile();
			if (item)
			{
				MaloW::Debug("Item UnEquipped " + item->GetItemName());
				std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)item->GetID());
				msg += this->zMsgHandler.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_AMMO);

				this->zServerChannel->Send(msg);
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
	
	if(this->zEng->GetKeyListener()->IsPressed('F'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_READY))
		{
			this->zKeyInfo.SetKeyState(KEY_READY, true);

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_PLAYER_READY);

			this->zServerChannel->Send(msg);
		}
		
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_READY))
		{
			this->zKeyInfo.SetKeyState(KEY_READY, false);
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
					//if (collisionObjects[0].type == OBJECT_TYPE_DYNAMIC_OBJECT || collisionObjects[0].type == OBJECT_TYPE_STATIC_OBJECT)
					//{
						this->SendPickupItemMessage(collisionObjects[0].gid.zID);
					//}
					//else if (collisionObjects[0].type == OBJECT_TYPE_DEAD_PLAYER)
					//{
					//	this->SendLootItemMessage(collisionObjects[0].owner, collisionObjects[0].gid.zID, collisionObjects[0].gid.zType);
					//}
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

					
					//Weapon* weapon = this->zPlayerInventory->GetRangedWeapon();
					//if (!weapon)
					//{
					//	this->DisplayMessageToClient("No Weapon is Equipped");
					//}
					//else
					//{
					//	std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_WEAPON_USE, (float)weapon->GetID());
					//	this->zServerChannel->Send(msg);
					//}
					std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_WEAPON_USE, 0.0f);
					this->zServerChannel->Send(msg);
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
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_RANGED, ITEM_SUB_TYPE_BOW);
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
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_PROJECTILE, ITEM_SUB_TYPE_ARROW);
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
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_PROJECTILE, ITEM_SUB_TYPE_ROCK);
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
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_MELEE, ITEM_SUB_TYPE_AXE);
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
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_MELEE, ITEM_SUB_TYPE_POCKET_KNIFE);
			if (item)
			{
				SendUseItemMessage(item->GetID());
			}

			this->zKeyInfo.SetKeyState(KEY_EQUIP, true);
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
	//Graphical error Object debug
	if (this->zEng->GetKeyListener()->IsPressed(VK_F1))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Graphical Terrain error at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Graphical error Object debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F2))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Graphical Object error at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Player Movement debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F3))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Player Movement blocked when shouldn't at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Player Movement debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F4))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Player movement Should be blocked at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Object debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F5))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "AI Is Blocked but shouldn't be at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Object debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F6))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "AI should be blocked at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << "" <<std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Misc debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F7))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Misc Error at " << std::endl;
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
	this->zServerChannel->Send(this->zMsgHandler.Convert(MESSAGE_TYPE_PING));
}

void Client::HandleNetworkMessage( const std::string& msg )
{
	std::vector<std::string> msgArray;
	msgArray = this->zMsgHandler.SplitMessage(msg);

	//Messages::Debug(msg);

	//Checks what type of message was sent
	if(msg.find(M_PING.c_str()) == 0)
	{
		this->Ping();
	}
	//WorldObjects
	else if(msg.find(M_UPDATE_ACTOR.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_ACTOR, msgArray[0]);
		this->UpdateActor(msgArray, id);
	}
	else if(msg.find(M_NEW_ACTOR.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_ACTOR, msgArray[0]);
		this->AddActor(msgArray, id);
	}
	//WorldObjects
	else if(msg.find(M_REMOVE_ACTOR.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_ACTOR, msgArray[0]);
		this->UpdateActor(msgArray, id);
	}
	else if(msg.find(M_EQUIP_ITEM.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_EQUIP_ITEM, msgArray[0]);
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
	else if(msg.find(M_ITEM_USE.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
		this->HandleUseItem(id);
	}
	else if(msg.find(M_REMOVE_EQUIPMENT.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_EQUIPMENT, msgArray[0]);
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
	else if(msg.find(M_UNEQUIP_ITEM.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
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
	else if(msg.find(M_ADD_INVENTORY_ITEM.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ADD_INVENTORY_ITEM, msgArray[0]);
		this->HandleAddInventoryItem(msgArray, id);
	}
	else if(msg.find(M_REMOVE_INVENTORY_ITEM.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_INVENTORY_ITEM, msgArray[0]);
		this->HandleRemoveInventoryItem(id);
	}
	else if(msg.find(M_WEAPON_USE.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);
		this->HandleWeaponUse(id);
	}
	else if(msg.find(M_SELF_ID.c_str()) == 0)
	{
		this->zID = this->zMsgHandler.ConvertStringToInt(M_SELF_ID, msgArray[0]);
	}
	else if(msg.find(M_CONNECTED.c_str()) == 0)
	{
		Vector3 camDir = this->zEng->GetCamera()->GetForward();
		Vector3 camUp = this->zEng->GetCamera()->GetUpVector();

		std::string serverMessage = "";
		serverMessage = this->zMsgHandler.Convert(MESSAGE_TYPE_USER_DATA);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_MESH_MODEL, this->zMeshID);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_DIRECTION, camDir);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_UP, camUp);

		this->zServerChannel->Send(serverMessage);
	}
	else if(msg.find(M_LOAD_MAP.c_str()) == 0)
	{
		std::string mapName = this->zMsgHandler.ConvertStringToSubstring(M_LOAD_MAP, msgArray[0]);

		this->InitGraphics(mapName);
	}
	else if(msg.find(M_ERROR_MESSAGE.c_str()) == 0)
	{
		std::string error_Message = this->zMsgHandler.ConvertStringToSubstring(M_ERROR_MESSAGE, msgArray[0]);
		DisplayMessageToClient(error_Message);
	}
	else if(msg.find(M_SERVER_FULL.c_str()) == 0)
	{
		this->CloseConnection("Server is full");
	}
	else if(msg.find(M_KICKED.c_str()) == 0)
	{
		this->CloseConnection("You got kicked");
	}
	else if(msg.find(M_SERVER_SHUTDOWN.c_str()) == 0)
	{
		this->CloseConnection("Server Shutdown");
	}
	else if(msg.find(M_START_GAME.c_str()) == 0)
	{
		this->zGameStarted = true;
	}
	else
	{
		MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[0] + " in HandleNetworkMessage");
		MaloW::Debug("C: " + msg);
	}
}

void Client::CloseConnection(const std::string& reason)
{
	MaloW::Debug("Client Shutdown: " + reason);
	//Todo Skriv ut vilket reason som gavs
	this->zServerChannel->TrySend(this->zMsgHandler.Convert(MESSAGE_TYPE_CONNECTION_CLOSED, (float)this->zID));
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
	/*std::vector<StaticObject*> staticObjects = this->zObjectManager->GetStaticObjects();
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
	*/
	return Collisions;
}

bool Client::CheckCollision()
{
	/*
	int position = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, this->zID);

	if (position == -1)
		return false;

	CollisionData data;

	iMesh* playerMesh = this->zObjectManager->GetPlayerObject(position)->GetMesh();

	if (!playerMesh)
		return false;
	*/
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
		this->zWorldRenderer = new WorldRenderer(WLE->world, GetGraphics());
	}
	else if ( WorldDeletedEvent* WDE = dynamic_cast<WorldDeletedEvent*>(e) )
	{
		if ( zWorldRenderer ) delete zWorldRenderer, zWorldRenderer = 0;
		if ( zAnchor ) zWorld->DeleteAnchor( zAnchor );
		if ( zWorld ) zWorld = 0;
	}
}