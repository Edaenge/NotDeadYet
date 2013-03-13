#include "Client.h"
#include "Graphics.h"
#include <NetworkPacket.h>
#include <ClientServerMessages.h>
#include <ClientServerMessages.h>
#include <World/EntityList.h>
#include <World/Entity.h>
#include "DebugMessages.h"
#include <DisconnectedEvent.h>
#include "PlayerConfig/PlayerSettings.h"
#include <algorithm>
#include <ctime>

using namespace MaloW;

// Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;

Client::Client()
{
	Messages::ClearDebug();

	this->zID = 0;
	this->zIP = "";
	this->zPort = 0;
	this->zActorType = NONE;
	this->zRunning = true;
	this->zCreated = false;
	this->zReady = false;
	this->zGameStarted = false;
	this->zShowCursor = false;
	this->zFrameTime = 0.0f;
	this->zTimeSinceLastPing = 0.0f;
	this->zMeshID	= GetPlayerSettings()->GetPlayerModel();
	this->zName		= GetPlayerSettings()->GetPlayerName();

	//Temporary Ghost Model
	this->zMeshCameraOffsets["media/models/ghost.obj"] = Vector3();
	this->zMeshCameraOffsets["media/models/token_anims.fbx"] = Vector3(0.0f, 2.3f, 0.0f);
	this->zMeshCameraOffsets["media/models/deer_temp.obj"] = Vector3(0.0f, 1.7f, 0.0f);
	this->zMeshCameraOffsets["media/models/temp_guy_movement_anims.fbx"] = Vector3(0.0f, 2.3f, 0.0f);

	this->zStateCameraOffset[STATE_IDLE] = Vector3(0.0f, 0.0f, 0.0f);
	this->zStateCameraOffset[STATE_RUNNING] = Vector3(0.0f, 0.0f, 0.0f);
	this->zStateCameraOffset[STATE_WALKING] = Vector3(0.0f, 0.0f, 0.0f);
	this->zStateCameraOffset[STATE_CROUCHING] = Vector3(0.0f, 1.0f, 0.0f);

	this->zAnimationFileReader[0] = AnimationFileReader("media/models/token_anims.cfg");

	this->zModelToReaderMap["media/models/token_anims.fbx"] = zAnimationFileReader[0];

	this->zSendUpdateDelayTimer = 0.0f;

	this->zEng = GetGraphics();
	
	this->zGuiManager = NULL;
	this->zActorManager = NULL;
	this->zServerChannel = NULL;
	this->zPlayerInventory = NULL;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	
	this->zMsgHandler = NetworkMessageConverter();

	this->zLatencyText = NULL;
	this->zServerUpsText = NULL;
	this->zClientUpsText = NULL;
	this->zWorld = NULL;
	this->zWorldRenderer = NULL;
	this->zAnchor = NULL;
	this->zCrossHair = NULL;
	this->zDamageIndicator = NULL;
	this->zBleedingAndHealthIndicator = NULL;
	this->zDamageOpacity = 0.0f;

	this->zPulseCounter = 0.0f;
	this->zPulsingTime = 8.0f;
	this->zHealthOpacity = 0.0f;
	this->zBleedingOpacity = 0.0f;
	this->zBleedingLevel = 0.0f;
	this->zDroppingPulse = false;
	this->zCurrentOffset = 0.0f;

	this->zEnergy = 100.0f;
	this->zStamina = 100.0f;
	this->zHealth = 100.0f;
	this->zHydration = 100.0f;
	this->zHunger = 100.0f;	
	
	this->zIgm = new InGameMenu();
	this->zPam = new PickAnimalMenu();

	this->zGameTimer = new GameTimer();
	this->zPerf = new MaloWPerformance();
	this->zPerf->SetFilePath("MPR_Client.txt");

	InitCraftingRecipes();
}

bool Client::Connect(const std::string &IPAddress, const unsigned int &port)
{
	bool result;
	this->zIP = IPAddress;
	this->zPort = port;
	this->zServerChannel = new ServerChannel(this);
	
	result = this->zServerChannel->Connect(IPAddress, port);
	if (result)
		this->zServerChannel->Start();

	return result;
}

Client::~Client()
{
	this->zPerf->PreMeasure("Deleting Client", 4);
	this->zEng->GetCamera()->RemoveMesh();

	this->Close();
	this->WaitUntillDone();

	SAFE_DELETE(this->zGuiManager);
	SAFE_DELETE(this->zActorManager);
	SAFE_DELETE(this->zServerChannel);
	SAFE_DELETE(this->zPlayerInventory);

	SAFE_DELETE(this->zIgm);
	SAFE_DELETE(this->zPam);

	SAFE_DELETE(this->zWorld);
	SAFE_DELETE(this->zGameTimer);

	this->zMeshCameraOffsets.clear();
	this->zStateCameraOffset.clear();

	if (this->zBlackImage)
		this->zEng->DeleteImage(this->zBlackImage);
	
	if (this->zDamageIndicator)
		this->zEng->DeleteImage(this->zDamageIndicator);

	if (this->zBleedingAndHealthIndicator)
		this->zEng->DeleteImage(this->zBleedingAndHealthIndicator);

	if (this->zCrossHair) 
		this->zEng->DeleteImage(this->zCrossHair);
	
	if (this->zClientUpsText)
		this->zEng->DeleteText(this->zClientUpsText);

	if (this->zServerUpsText)
		this->zEng->DeleteText(this->zServerUpsText);

	if (this->zLatencyText)
		this->zEng->DeleteText(this->zLatencyText);

	auto it_text_end = this->zDisplayedText.end();
	for (auto it = this->zDisplayedText.begin(); it != it_text_end; it++)
	{
		TextDisplay* temp = (*it);

		this->zEng->DeleteText(temp->zText);
		SAFE_DELETE(temp);
	}

	this->zDisplayedText.clear();
	this->zPerf->PostMeasure("Deleting Client", 4);

	this->zPerf->GenerateReport(this->zEng->GetEngineParameters());
	SAFE_DELETE(this->zPerf);

	FreeCraftingRecipes();
}

void Client::Update()
{
	this->UpdateText();

	this->zPerf->PreMeasure("Actor Updates", 0);
	this->zActorManager->UpdateObjects(this->zGameTimer->GetDeltaTime(), this->zID, this->zWorld);
	this->zPerf->PostMeasure("Actor Updates", 0);

	this->zPerf->PreMeasure("Gui Updates", 3);
	if (this->zGuiManager)
		this->zGuiManager->Update(this->zDeltaTime);
	this->zPerf->PostMeasure("Gui Updates", 3);

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
		this->zPerf->PreMeasure("World Updates", 2);
		this->zWorld->Update();
		this->zPerf->PostMeasure("World Updates", 2);

		this->zPerf->PreMeasure("WorldRenderer Updates", 0);
		if ( zWorldRenderer ) 
			zWorldRenderer->Update();
		this->zPerf->PostMeasure("WorldRenderer Updates", 0);

	//	this->IgnoreRender( 50.0f, zEng->GetCamera()->GetPosition().GetXZ() );
	}		

	if(this->zHealth > 0.0f)
	{
		this->UpdateHealthAndBleedingImage();
	}

	this->zDamageOpacity -= this->zDeltaTime * 0.25f;

	if(this->zDamageIndicator)
	{
		this->zDamageIndicator->SetOpacity(this->zDamageOpacity);
	}

	if(this->zDamageOpacity < 0.0f)
	{
		this->zDamageOpacity = 0.0f;
		if(this->zDamageIndicator)
		{
			this->zEng->DeleteImage(this->zDamageIndicator);
			this->zDamageIndicator = NULL;
		}
	}

}

void Client::InitGraphics(const std::string& mapName)
{
	if (!this->zActorManager)
		this->zActorManager = new ClientActorManager();
	else
		this->zActorManager->ClearAll();

	if (!this->zPlayerInventory)
		this->zPlayerInventory = new Inventory();
	else
		this->zPlayerInventory->ClearAll();

	if (!this->zGuiManager)
		this->zGuiManager = new GuiManager(this->zEng);
	else
		this->zGuiManager->ResetGui();
	
	this->zActorManager->SetFBXMapping(this->zModelToReaderMap);

	LoadEntList("Entities.txt");

	if ( zWorld ) 
		delete zWorld, zWorld=0;
	try
	{
		this->zWorld = new World(this, mapName, true);
	}
	catch (char* s)
	{
		std::string errorMessage = s;
		if (errorMessage == "Empty File!")
		{
			errorMessage = "Missing map: " + mapName;
		}
		else if(errorMessage == "File Doesn't Have Header!")
		{
			errorMessage = "Map: " + mapName + " Could be corrupt";
		}

		this->CloseConnection(errorMessage);
		return;
	}
	catch (...)
	{
		this->CloseConnection("Map Not Found");
		return;
	}
	
	Vector2 center = this->zWorld->GetWorldCenter();

	this->zEng->GetCamera()->SetPosition( Vector3(center.x, 20, center.y) );
	this->zEng->GetCamera()->LookAt( this->zEng->GetCamera()->GetPosition() + Vector3(1, 0, 0) );

	this->zAnchor = this->zWorld->CreateAnchor();
	this->zAnchor->position = center;
	this->zAnchor->radius = this->zEng->GetEngineParameters().FarClip;
	
	int windowWidth = this->zEng->GetEngineParameters().WindowWidth;
	int windowHeight = this->zEng->GetEngineParameters().WindowHeight;	
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;
	float length = ((25.0f / 1024.0f) * dx);
	float xPos = offSet + (0.5f * dx) - length * 0.5f;
	float yPos = (windowHeight / 2.0f) - length * 0.5f;

	this->zWorld->Update();
	for(int i = 0; i < 50; i++)
	{
		this->zWorldRenderer->Update();
	}
	
	
	if (this->zCrossHair)
		this->zEng->DeleteImage(this->zCrossHair);

	this->zCrossHair = this->zEng->CreateImage(Vector2(xPos, yPos), Vector2(length, length), "Media/Icons/cross.png");
	this->zCrossHair->SetOpacity(0.5f);

	if (this->zLatencyText)
		this->zEng->DeleteText(this->zLatencyText);

	this->zLatencyText = this->zEng->CreateText("", Vector2(1, 1), 1.0f, "Media/Fonts/new");

	if (this->zServerUpsText)
		this->zEng->DeleteText(this->zServerUpsText);

	this->zServerUpsText = this->zEng->CreateText("", Vector2(1, 25), 1.0f, "Media/Fonts/new");

	if (this->zClientUpsText)
		this->zEng->DeleteText(this->zClientUpsText);

	this->zClientUpsText = this->zEng->CreateText("", Vector2(1, 49), 1.0f, "Media/Fonts/new");
}

void Client::CalculateDeltaTime()
{
	this->zDeltaTime = this->zGameTimer->Frame();

	//20 fps Minimum cap
	static const float MAXIMUM_DELTA = 1.0f / 20.0f;
	static float accumulator = 0.0f;
	static float difference = 0.0f;

	//Check if Fps is Higher than minimum cap
	if (this->zDeltaTime > MAXIMUM_DELTA)
	{
		//Add difference to buffer.
		float currentDifference = this->zDeltaTime - MAXIMUM_DELTA;
		accumulator += currentDifference;
		//Decrease deltaTime so it doesn't go below limit.
		this->zDeltaTime -= currentDifference;
	}
	else
	{
		//Calculate difference Between current delta time and minimum limit.
		difference = MAXIMUM_DELTA - this->zDeltaTime;

		//Check if Difference is higher than what is stored in Buffer.
		if (difference > accumulator)
		{
			//Add buffer to current deltaTime.
			this->zDeltaTime += accumulator;
			accumulator = 0.0f;
		}
		else
		{
			//Add difference to current deltaTime and decrease difference from buffer.
			this->zDeltaTime = difference;
			accumulator -= difference;
		}
	}
	this->zGameTimer->CalculateFps(this->zDeltaTime);
}

void Client::Life()
{
	MaloW::Debug("Client Process Started");
	
	static const float FRAME_TIME = 60.0f;
	static const float TARGET_DT = 1.0f / FRAME_TIME;

	this->zGameTimer->Init();
	while(this->zEng->IsRunning() && this->stayAlive)
	{
		this->CalculateDeltaTime();

		this->UpdateGame();

		if (FRAME_TIME > 0)
		{
			if (this->zDeltaTime < TARGET_DT)
			{
				float sleepTime = (TARGET_DT - this->zDeltaTime) * 1000.0f;
				Sleep((DWORD)sleepTime);
			}
		}
	}

	this->zRunning = false;
}

void Client::UpdateGame()
{
	// 50 updates per second
	static const float UPDATE_DELAY = 0.020f;
	static const float FPS_DELAY = 1.0f;
	static float fps_Delay_Timer = 0.0f;

	this->CheckKeyboardInput();
	if(this->zCreated)
	{
		this->zSendUpdateDelayTimer += this->zDeltaTime;
		this->zTimeSinceLastPing += this->zDeltaTime;
		fps_Delay_Timer += this->zDeltaTime;
		
		if (fps_Delay_Timer >= FPS_DELAY)
		{
			std::stringstream ss;
			ss << this->zGameTimer->GetFPS() <<" CLIENT FPS";
			this->zClientUpsText->SetText(ss.str().c_str());

			if (this->zGameTimer->GetFPS() < 30)
				this->zClientUpsText->SetColor(Vector3(0.0f, -255.0f, -255.0f));
			else if (this->zGameTimer->GetFPS() > 30 && this->zGameTimer->GetFPS() < 60)
				this->zClientUpsText->SetColor(Vector3(0.0f, 0.0f, -255.0f));
			else
				this->zClientUpsText->SetColor(Vector3(-255.0f, 0.0f, -255.0f));

			fps_Delay_Timer = 0.0f;
		}

		this->zActorManager->SetUpdatesPerSec(this->zGameTimer->GetFPS());

		if(this->zSendUpdateDelayTimer >= UPDATE_DELAY)
		{
			this->zSendUpdateDelayTimer = 0.0f;

			this->SendClientUpdate();
		}

		this->Update();
	}
	else
	{
			this->zHealth = 0.0f;
			this->zBleedingLevel = 0.0f;
			this->zCurrentOffset = 0.0f;

			if(this->zDamageIndicator)
			{
				this->zEng->DeleteImage(this->zDamageIndicator);
				this->zDamageIndicator = NULL;
			}
			if(this->zBleedingAndHealthIndicator)
			{
				this->zEng->DeleteImage(this->zBleedingAndHealthIndicator);
				this->zBleedingAndHealthIndicator = NULL;
			}

	}

	AudioManager* am = AudioManager::GetInstance();
	am->Update();

	this->zPerf->PreMeasure("Message Reading", 0);

	this->ReadMessages();

	this->zPerf->PostMeasure("Message Reading", 0);
	this->CheckMenus();
}

void Client::CheckMenus()
{
	if(this->zPam->GetShow())
	{
		int returnValue = this->zPam->Run((int)this->zEnergy);
		if(returnValue == DEER)
		{
			this->zPam->ToggleMenu();
			zShowCursor = this->zPam->GetShow();

			// MAKE ME A DEER.
			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_PLAY_AS_ANIMAL, 0);
			this->zServerChannel->Send(msg);
		}
		if(returnValue == BEAR)
		{
			this->zPam->ToggleMenu();
			zShowCursor = this->zPam->GetShow();

			// MAKE ME A BEAR.
			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_PLAY_AS_ANIMAL, 2);
			this->zServerChannel->Send(msg);
		}
	}
	if (this->zIgm->GetShow())
	{
		int returnValue = this->zIgm->Run();
		if(returnValue == IGQUIT)
		{
			this->stayAlive = false;
			this->CloseConnection("");
		}
		else if(returnValue == IGRESUME)
		{
			this->zIgm->SetShow(false);
			zShowCursor = false;
			this->zEng->GetKeyListener()->SetMousePosition(
				Vector2((float)(this->zEng->GetEngineParameters().WindowWidth/2), 
				(float)(this->zEng->GetEngineParameters().WindowHeight/2)));
		}
	}
}

void Client::ReadMessages()
{
	static const unsigned int MAX_NR_OF_MESSAGES = 10000;
	int nrOfMessages = this->GetEventQueueSize();
	if (nrOfMessages == 0)
		return;

	int messages_To_Read = min(MAX_NR_OF_MESSAGES, nrOfMessages);
	for (int i = 0; i < messages_To_Read && this->stayAlive; i++)
	{
		if (MaloW::ProcessEvent* ev = this->PeekEvent())
		{
			//Check if Client has received a Message
			if ( MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(ev) )
			{
				this->HandleNetworkMessage(np->GetMessage());
			}
			else if ( DisconnectedEvent* np = dynamic_cast<DisconnectedEvent*>(ev) )
			{
				this->AddDisplayText(np->GetReason(), true);
				Sleep(5000);
				this->CloseConnection(np->GetReason());
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

	Actor* player = this->zActorManager->GetActor(this->zID);
	
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

void Client::UpdateMeshRotation()
{
	/*Actor* player = this->zActorManager->GetActor(this->zID);
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
	playerMesh->RotateAxis(around, angle);*/
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
	if (!zShowCursor)
	{
		this->CheckKey(KEY_FORWARD);
		this->CheckKey(KEY_BACKWARD);

		this->CheckKey(KEY_LEFT);
		this->CheckKey(KEY_RIGHT);
	}
}

void Client::CheckPlayerSpecificKeys()
{
	if(this->zGuiManager->IsInventoryOpen() || this->zGuiManager->IsLootingOpen())
	{
		Menu_select_data msd;
		msd = this->zGuiManager->CheckCollisionInv(); // Returns -1 on both values if no hits.

		if (msd.zAction != -1 && msd.gid.zID != -1)
		{
			Item* item = this->zPlayerInventory->SearchAndGetItem(msd.gid.zID);
			if (msd.zAction == USE)
			{
				if (item)
					SendUseItemMessage(msd.gid.zID);
			}
			else if(msd.zAction == EQUIP)
			{
				if(item)
					SendEquipItem(msd.gid.zID);
			}
			else if (msd.zAction == LOOT)
			{
				unsigned int id = this->zGuiManager->GetLootingActor();
				if (id != 0)
					SendLootItemMessage(id, msd.gid.zID, msd.gid.zType, msd.gid.zSubType);
			}
			else if (msd.zAction == FILL)
			{
				this->SendItemFill(msd.gid.zID);
			}
			else if (msd.zAction == DROP)
			{
				if(item)
					this->SendDropItemMessage(msd.gid.zID);
			}
			else if (msd.zAction == UNEQUIP)
			{
				if(item)
					this->SendUnEquipItem(msd.gid.zID);
			}
			else if (msd.zAction == CRAFT)
			{
				if(!this->zGuiManager->IsCraftOpen())
				{
					if(this->zGuiManager->IsLootingOpen())
						this->zGuiManager->ToggleLootGui(0);

					this->zGuiManager->ToggleCraftingGui();
					this->zShowCursor = true;
				}
			}
		}
	}
	if(this->zGuiManager->IsCraftOpen())
	{
		Menu_select_data msd = this->zGuiManager->CheckCrafting();
		if (msd.zAction == CRAFT)
		{
			if (!this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, true);
				this->SendCraftItemMessage(msd.gid.zType, msd.gid.zSubType);
			}
		}
		else
		{
			this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, false);
		}
	}

	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INTERACT)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_INTERACT))
		{
			std::string msg =  "";
			std::vector<unsigned int> collisionObjects = this->RayVsWorld();
			if (collisionObjects.size() > 0)
			{
				auto it_collision_end = collisionObjects.end();
				for (auto it = collisionObjects.begin(); it != it_collision_end; it++)
				{
					msg += this->zMsgHandler.Convert(MESSAGE_TYPE_LOOT_OBJECT, (float)(*it));
				}
				this->zServerChannel->Send(msg);
			}
			else
			{
				msg = "No Lootable Objects Found.";
				this->AddDisplayText(msg, true);
			}
			this->zKeyInfo.SetKeyState(KEY_INTERACT, true);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_INTERACT))
		{
			if(this->zGuiManager->IsInventoryOpen())
				this->zGuiManager->ToggleInventoryGui();

			if(this->zGuiManager->IsLootingOpen())
				this->zGuiManager->ToggleLootGui(0);
			
			this->zGuiManager->ResetLoot();
			this->zShowCursor = false;
			this->zKeyInfo.SetKeyState(KEY_INTERACT, false);
		}
	}
	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_CRAFTING)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_CRAFTING))
		{
			if(!this->zGuiManager->IsCraftOpen())
			{
				if(this->zGuiManager->IsLootingOpen())
					this->zGuiManager->ToggleLootGui(0);

				this->zGuiManager->ToggleCraftingGui();
				this->zShowCursor = true;
			}
			else
			{
				this->zGuiManager->ToggleCraftingGui();
				if(!this->zGuiManager->IsLootingOpen() && !this->zGuiManager->IsInventoryOpen())
					this->zShowCursor = false;
			}
			this->zKeyInfo.SetKeyState(KEY_CRAFTING, true);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_CRAFTING))
			this->zKeyInfo.SetKeyState(KEY_CRAFTING, false);

	}

	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_SWAP_EQ)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_SWAP_EQ))
		{
			this->zKeyInfo.SetKeyState(KEY_SWAP_EQ, true);

			if(this->zPlayerInventory->SwapWeapon())
			{
				std::string msg;
				msg = this->zMsgHandler.Convert(MESSAGE_TYPE_WEAPON_EQUIPMENT_SWAP);
				this->zServerChannel->Send(msg);
			}
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_SWAP_EQ))
			this->zKeyInfo.SetKeyState(KEY_SWAP_EQ, false);
	}

	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INVENTORY)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_INVENTORY) && !this->zPam->GetShow())
		{
			this->zKeyInfo.SetKeyState(KEY_INVENTORY, true);
			if (!this->zGuiManager->IsInventoryOpen())
			{
				this->zShowCursor = true;
				this->zGuiManager->ToggleInventoryGui();
			}
			else if(this->zGuiManager->IsInventoryOpen())
			{
				this->zGuiManager->ToggleInventoryGui();
				if(!this->zGuiManager->IsLootingOpen() && !this->zGuiManager->IsCraftOpen())
					this->zShowCursor = false;
			}
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_INVENTORY))
			this->zKeyInfo.SetKeyState(KEY_INVENTORY, false);
	}

	if (!zShowCursor)
	{
		if (this->zEng->GetKeyListener()->IsClicked(1))
		{
			if (!this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, true);
				std::string msg = "";
				msg = this->zMsgHandler.Convert(MESSAGE_TYPE_KEY_DOWN, (float)MOUSE_LEFT_PRESS);

				this->zServerChannel->Send(msg);
			}
		}
		else
		{
			if (this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, false);

				Item* primaryWeapon = this->zPlayerInventory->GetPrimaryEquip();
				if (!primaryWeapon)
				{
					this->DisplayMessageToClient("No Weapon is Equipped", true);
				}
				else
				{
					std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_WEAPON_USE, (float)primaryWeapon->GetID());
					this->zServerChannel->Send(msg);
				}
			}
		}
	}
	this->HandleWeaponEquips();
}

void Client::CheckGhostSpecificKeys()
{
	this->CheckKey(KEY_JUMP);
	this->CheckKey(KEY_DUCK);

	// Opens pick menu if you can do it.
	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_PICKMENU)))
	{
		if(!this->zKeyInfo.GetKeyState(KEY_PICKMENU))
		{
			this->zKeyInfo.SetKeyState(KEY_PICKMENU, true);
			this->zPam->ToggleMenu(); // Shows the menu and sets Show to true.
			if(this->zPam->GetShow())
				zShowCursor = true;
			else
				zShowCursor = false;
		}
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_PICKMENU))
			this->zKeyInfo.SetKeyState(KEY_PICKMENU, false);

		if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INTERACT)) )
		{
			// Check for targets to possess!

			std::vector<unsigned int> ids;

			ids = this->RayVsWorld();

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_TRY_TO_POSSESS_ANIMAL, 0);
			this->zServerChannel->Send(msg);
		}


	}
}

void Client::CheckNonGhostInput()
{
	this->CheckKey(KEY_SPRINT);

	this->CheckKey(KEY_DUCK);

	//Kill yourself button
	if (this->zEng->GetKeyListener()->IsPressed(VK_CONTROL) && this->zEng->GetKeyListener()->IsPressed('K'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_KILL))
		{
			this->zKeyInfo.SetKeyState(KEY_KILL, true);

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ACTOR_KILL);

			this->zServerChannel->Send(msg);
		}
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_KILL))
			this->zKeyInfo.SetKeyState(KEY_KILL, false);
	}
}

void Client::CheckKeyboardInput()
{
	if (this->zCreated && this->zGameStarted)
	{
		MaloW::Debug("Something Went Wrong. This player cannot be found. In function Client::HandleKeyBoardInput");
		return;
	}

	else if (this->zEng->GetKeyListener()->IsPressed(VK_CONTROL)  && this->zEng->GetKeyListener()->IsPressed(VK_MENU) && this->zEng->GetKeyListener()->IsPressed('R'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_RESTART))
		{
			this->zKeyInfo.SetKeyState(KEY_RESTART, true);

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_RESTART_GAME_REQUEST);

			this->zServerChannel->Send(msg);
		}
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_RESTART))
			this->zKeyInfo.SetKeyState(KEY_RESTART, false);
	}

	if (this->zActorType != NONE)
	{
		this->CheckMovementKeys();
	}

	if (this->zActorType == HUMAN)
	{
		this->CheckPlayerSpecificKeys();
	}
	else if (this->zActorType == GHOST)
	{
		this->CheckGhostSpecificKeys();
	}
	else if (this->zActorType == ANIMAL)
	{
		this->CheckAnimalInput();
	}

	if (this->zActorType != GHOST)
	{
		this->CheckNonGhostInput();
	}

	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_MENU)))
	{
		if(!this->zKeyInfo.GetKeyState(KEY_MENU))
		{
			this->zKeyInfo.SetKeyState(KEY_MENU, true);
			if(!this->zIgm->GetShow())
			{
				
				if(this->zPam->GetShow())
				{
					this->zPam->ToggleMenu();
				}
				if(this->zGuiManager->IsCraftOpen())
				{
					this->zGuiManager->ToggleCraftingGui();
				}
				if(this->zGuiManager->IsLootingOpen())
				{
					this->zGuiManager->ToggleLootGui(0);
				}
				if(this->zGuiManager->IsInventoryOpen())
				{
					this->zGuiManager->ToggleInventoryGui();
				}

				this->zIgm->ToggleMenu(); // Shows the menu and sets Show to true.
				zShowCursor = true;
			}
		}
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_MENU))
			this->zKeyInfo.SetKeyState(KEY_MENU, false);
	}

	//Tell Server Client is Ready
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
			this->zKeyInfo.SetKeyState(KEY_READY, false);
	}

	if (this->zEng->GetKeyListener()->IsPressed('Z'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);

			this->zReady = true;
		}
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_TEST))
			this->zKeyInfo.SetKeyState(KEY_TEST, false);
	}
	
	this->HandleDebugInfo();
}

void Client::CheckAnimalInput()
{
	this->CheckKey(KEY_JUMP);

	//Leave Animal An Become a Ghost again
	if (this->zEng->GetKeyListener()->IsPressed(VK_CONTROL) && this->zEng->GetKeyListener()->IsPressed('G'))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_TEST))
		{
			this->zKeyInfo.SetKeyState(KEY_TEST, true);

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_LEAVE_ANIMAL);

			this->zServerChannel->Send(msg);
		}
	}
	else
	{
		if(this->zKeyInfo.GetKeyState(KEY_TEST))
			this->zKeyInfo.SetKeyState(KEY_TEST, false);
	}

	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INTERACT)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_INTERACT))
		{
			std::vector<unsigned int> collisionObjects = this->RayVsWorld();
			if (collisionObjects.size() > 0)
			{
				std::string msg = "";
				auto it_collision_end = collisionObjects.end();
				for (auto it = collisionObjects.begin(); it != collisionObjects.end(); it++)
				{
					msg += this->zMsgHandler.Convert(MESSAGE_TYPE_DEER_EAT_OBJECT, (float)(*it));
				}
				this->zServerChannel->Send(msg);
			}
			this->zKeyInfo.SetKeyState(KEY_INTERACT, true);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_INTERACT))
		{
			this->zKeyInfo.SetKeyState(KEY_INTERACT, false);
		}
	}

	if (this->zEng->GetKeyListener()->IsClicked(1))
	{
		if (!this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
		{
			this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, true);

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ANIMAL_ATTACK, (float)MOUSE_LEFT_PRESS);
			this->zServerChannel->Send(msg);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
			this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, false);
	}

	if (this->zEng->GetKeyListener()->IsClicked(2))
	{
		if (!this->zKeyInfo.GetKeyState(MOUSE_RIGHT_PRESS))
		{
			this->zKeyInfo.SetKeyState(MOUSE_RIGHT_PRESS, true);

			std::string msg = this->zMsgHandler.Convert(MESSAGE_TYPE_ANIMAL_ATTACK, (float)MOUSE_RIGHT_PRESS);
			this->zServerChannel->Send(msg);
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(MOUSE_RIGHT_PRESS))
			this->zKeyInfo.SetKeyState(MOUSE_RIGHT_PRESS, false);
	}
}

//Use to equip weapon with keyboard
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
			Item* item = this->zPlayerInventory->SearchAndGetItemFromType(ITEM_TYPE_WEAPON_MELEE, ITEM_SUB_TYPE_MACHETE);
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
	time_t t = time(0);

	struct tm * now = localtime(&t);

	//Graphical error Terrain debug
	if (this->zEng->GetKeyListener()->IsPressed(VK_F1))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
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
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
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
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
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
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
			ss << "Player movement Should be blocked at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//AI debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F5))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
			ss << "AI Is Blocked but shouldn't be at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//AI debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F6))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
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
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
			ss << "Misc Error at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Sound debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F8))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
			ss << "Sound Error at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Terrain Normals debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F9))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
			ss << "Terrain Normal Error at " << std::endl;
			ss << "Camera Position = (" << position.x <<", " <<position.y <<", " <<position.z << ") " << std::endl;
			ss << "Camera Direction = (" << direction.x <<", " <<direction.y <<", " <<direction.z << ") " << std::endl;
			ss << std::endl;

			DebugMsg::Debug(ss.str());

			this->zKeyInfo.SetKeyState(KEY_DEBUG_INFO, true);
		}
	}
	//Water debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F10))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
			ss << "Created on " << now->tm_year + 1900 << "-" << now->tm_mon + 1 << "-" << now->tm_mday <<std::endl;
			ss << "Water Error at " << std::endl;
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

void Client::PingAck(float serverTime)
{
	this->zTimeSinceLastPing = 0.0f;
	this->zServerChannel->Send(this->zMsgHandler.Convert(MESSAGE_TYPE_PING, serverTime));
}

void Client::HandleNetworkPacket( Packet* P )
{
	if ( ServerFramePacket* SFP = dynamic_cast<ServerFramePacket*>(P) )
	{
		this->zPerf->PreMeasure("Network Actor Updating", 2);
		this->UpdateActors(SFP);
		this->zPerf->PostMeasure("Network Actor Updating", 2);
	}
	else if (NewActorPacket* NPA = dynamic_cast<NewActorPacket*>(P))
	{
		this->zPerf->PreMeasure("Network Actor Adding", 2);
		this->AddActor(NPA);
		this->zPerf->PostMeasure("Network Actor Adding", 2);
	}
	else if (PhysicalConditionPacket* PCP = dynamic_cast<PhysicalConditionPacket*>(P))
	{
		this->zPerf->PreMeasure("Network Physical Conditions Updating", 2);
		this->UpdatePhysicalCondition(PCP);
		this->zPerf->PostMeasure("Network Physical Conditions Updating", 2);
	}

	delete P;
	P = NULL;
}

void Client::HandleNetworkMessage( const std::string& msg )
{
	if ( msg.find("PACKET") == 0 )
	{
		std::stringstream ss(msg);
		ss.seekg(6);

		// Packet Size
		unsigned int packetTypeSize = 0;
		ss.read( reinterpret_cast<char*>(&packetTypeSize), sizeof(unsigned int) );

		// Packet Type
		std::string type;
		type.resize(packetTypeSize);
		ss.read(&type[0], packetTypeSize); 

		// TODO: Factory Pattern
		Packet* packet = NULL;

		if ( type == "ServerFramePacket" )
		{
			packet = new ServerFramePacket();
		}
		else if (type == "NewActorPacket")
		{
			packet = new NewActorPacket();
		}
		else if ( type == "PhysicalConditionPacket")
		{
			packet = new PhysicalConditionPacket();
		}

		if ( !packet ) 
			throw("Unknown Packet Type");

		// Deserialize
		packet->Deserialize(ss);
		this->zPerf->PreMeasure("Network Packet Handling", 1);

		// Handle
		HandleNetworkPacket(packet);

		this->zPerf->PostMeasure("Network Packet Handling", 1);
		return;
	}

	std::vector<std::string> msgArray;
	msgArray = this->zMsgHandler.SplitMessage(msg);

	if (Messages::MsgFileWrite())
		Messages::Debug(msg);
	this->zPerf->PreMeasure("Network Message Handling", 1);

	//Checks what type of message was sent
	if(msgArray[0].find(M_PING.c_str()) == 0)
	{
		float time = this->zMsgHandler.ConvertStringToFloat(M_PING, msgArray[0]);
		this->PingAck(time);
	}
	else if (msgArray[0].find(M_HEALTH) == 0)
	{
	}
	else if (msgArray[0].find(M_CLIENT_LATENCY.c_str()) == 0)
	{
		float latency = this->zMsgHandler.ConvertStringToFloat(M_CLIENT_LATENCY, msgArray[0]);

		std::stringstream ss;

		ss << (int)latency <<" MS";
		zLatencyText->SetText(ss.str().c_str());

		if (latency > 300)
			this->zLatencyText->SetColor(Vector3(0.0f, -255.0f, -255.0f));
		else if (latency > 200 && latency < 300)
			this->zLatencyText->SetColor(Vector3(0.0f, 0.0f, -255.0f));
		else
			this->zLatencyText->SetColor(Vector3(-255.0f, 0.0f, -255.0f));

		this->zActorManager->SetLatency((int)latency);
	}
	else if (msgArray[0].find(M_SERVER_UPDATES_PER_SEC.c_str()) == 0)
	{
		int updatesPerSec = this->zMsgHandler.ConvertStringToInt(M_SERVER_UPDATES_PER_SEC, msgArray[0]);

		std::stringstream ss;

		ss << updatesPerSec <<" SERVER FPS";
		if ( this->zServerUpsText )
		{
			this->zServerUpsText->SetText(ss.str().c_str());

			if (updatesPerSec < 30)
				this->zServerUpsText->SetColor(Vector3(0.0f, -255.0f, -255.0f));
			else if (updatesPerSec > 30 && updatesPerSec < 60)
				this->zServerUpsText->SetColor(Vector3(0.0f, 0.0f, -255.0f));
			else
				this->zServerUpsText->SetColor(Vector3(-255.0f, 0.0f, -255.0f));
		}

		if ( zActorManager )
		{
			this->zActorManager->SetUpdatesPerSec(updatesPerSec);
		}
	}
	else if (msgArray[0].find(M_SERVER_RESTART.c_str()) == 0)
	{
		this->zActorManager->ClearAll();
		
		if(this->zPlayerInventory)
			this->zPlayerInventory->ClearAll();
		else
			this->zPlayerInventory = new Inventory();

		//Could Crash
		if(this->zGuiManager)
			this->zGuiManager->ResetGui();
		else
			this->zGuiManager = new GuiManager(GetGraphics());

		this->zEng->GetCamera()->RemoveMesh();
	}
	else if (msgArray[0].find(M_FOG_ENCLOSEMENT.c_str()) == 0)
	{
		Vector2 center2D = this->zWorld->GetWorldCenter();
		Vector3 center = Vector3(center2D.x, 0.0f, center2D.y);

		float radius = this->zMsgHandler.ConvertStringToFloat(M_FOG_ENCLOSEMENT, msgArray[0]);
		this->zEng->SetEnclosingFogEffect(center, radius, 0.4f);
	}
	else if (msgArray[0].find(M_MESH_BINDING.c_str()) == 0)
	{
		unsigned int bone = this->zMsgHandler.ConvertStringToInt(M_MESH_BINDING, msgArray[0]);
		std::string model = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, msgArray[1]);
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_OBJECT_ID, msgArray[2]);

		Actor* actor = this->zActorManager->GetActor(id);
		
		if (actor)
		{
			auto it = this->zModelToReaderMap.find(actor->GetModel());

			if (it != this->zModelToReaderMap.end())
			{
				std::string boneName = it->second.GetBindingBone(bone);

				if (boneName != "")
				{
					iMesh* mesh = NULL;

					if (model.length() > 4)
					{
						std::string substr = model.substr(model.length() - 4);
						
						if (substr == ".obj")
							mesh = this->zEng->CreateStaticMesh(model.c_str(), Vector3());
						else if (substr == ".fbx")
							mesh = this->zEng->CreateFBXMesh(model.c_str(), Vector3());

					}
					
					if (mesh)
					{
						if (iFBXMesh* fbxMesh = dynamic_cast<iFBXMesh*>(actor->GetMesh()))
						{
							mesh->SetScale(Vector3(0.05f, 0.05f, 0.05f));
							if(fbxMesh->BindMesh(boneName.c_str(), mesh))
							{
								actor->AddSubMesh(model, mesh);
							}
						}
					}
				}
			}
		}
	}
	else if (msgArray[0].find(M_MESH_UNBIND.c_str()) == 0)
	{
		if (msgArray.size() > 1)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_MESH_UNBIND, msgArray[0]);
			std::string model = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, msgArray[1]);

			Actor* actor = this->zActorManager->GetActor(id);

			if (actor)
			{
				if (iFBXMesh* fbxMesh = dynamic_cast<iFBXMesh*>(actor->GetMesh()))
				{
					iMesh* mesh = actor->GetSubMesh(model);

					if (mesh)
					{
						mesh->SetScale(Vector3(0.05f, 0.05f, 0.05f));
						fbxMesh->UnbindMesh(mesh);
						
						this->zEng->DeleteMesh(mesh);
						actor->RemoveSubMesh(model);
					}
				}
			}
		}
	}
	//Actors
	//else if(msg.find(M_UPDATE_ACTOR.c_str()) == 0)
	//{
	//	unsigned int id = this->zMsgHandler.ConvertStringToInt(M_UPDATE_ACTOR, msgArray[0]);
	//	this->UpdateActor(msgArray, id);
	//}
	//else if(msgArray[0].find(M_NEW_ACTOR.c_str()) == 0)
	//{
	//	unsigned int id = this->zMsgHandler.ConvertStringToInt(M_NEW_ACTOR, msgArray[0]);
	//	this->AddActor(msgArray, id);
	//}
	else if (msgArray[0].find(M_ACTOR_TAKE_DAMAGE.c_str()) == 0)
	{
		if (msgArray.size() > 1)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ACTOR_TAKE_DAMAGE, msgArray[0]);
			float damageTaken = this->zMsgHandler.ConvertStringToFloat(M_HEALTH, msgArray[1]);
			this->HandleTakeDamage(id, damageTaken);
		}
	}
	else if (msgArray[0].find(M_SUN_DIRECTION.c_str()) == 0)
	{
		Vector3 sunLightDirection = this->zMsgHandler.ConvertStringToVector(M_SUN_DIRECTION, msgArray[0]);
		Vector3 sunLightColor = this->zEng->GetSunLightColor();
		float sunLightIntensity = this->zEng->GetSunLightIntensity();
		this->zEng->SetSunLightProperties(sunLightDirection, sunLightColor, sunLightIntensity);
	}
	else if (msgArray[0].find(M_DEAD_ACTOR.c_str()) == 0)
	{
		//unsigned int id = this->zMsgHandler.ConvertStringToInt(M_DEAD_ACTOR, msgArray[0]);
	}
	//Actors
	else if(msgArray[0].find(M_REMOVE_ACTOR.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_ACTOR, msgArray[0]);
		this->RemoveActor(id);
	}
	else if (this->zActorType == HUMAN && this->CheckHumanSpecificMessages(msgArray))
	{
		
	}
	else if(msgArray[0].find(M_PLAY_SOUND.c_str()) == 0)
	{
		int eventId = this->zMsgHandler.ConvertStringToInt(M_PLAY_SOUND, msgArray[0]);
		Vector3 pos = this->zMsgHandler.ConvertStringToVector(M_POSITION, msgArray[1]);

		AudioManager* am = AudioManager::GetInstance();
		IEventHandle* tempHandle;
		if(am->GetEventHandle(eventId, tempHandle) == FMOD_OK)
		{
			FMOD_VECTOR* temp = new FMOD_VECTOR;
			temp->x = pos.x;
			temp->y = pos.y;
			temp->z = pos.z;
			tempHandle->Setposition(temp);
			tempHandle->Play();
			delete temp;
		}
		delete tempHandle;
		tempHandle = NULL;
	}
	else if(msgArray[0].find(M_SELF_ID.c_str()) == 0)
	{
		//this->zEng->DeleteImage(this->zBleedingAndHealthIndicator);

		this->zID = this->zMsgHandler.ConvertStringToInt(M_SELF_ID, msgArray[0]);
		this->ResetPhysicalConditions();
		if (Actor* actor = this->zActorManager->GetActor(this->zID))
		{
			auto meshOffsetsIterator = this->zMeshCameraOffsets.find(actor->GetModel());
			if (meshOffsetsIterator != this->zMeshCameraOffsets.end())
			{
				this->zMeshOffset = meshOffsetsIterator->second;
			}
			else
			{
				this->zMeshOffset = Vector3();
			}
			this->zActorManager->SetCameraOffset(this->zMeshOffset);
			this->zCreated = true;
			this->zEng->GetCamera()->SetMesh(actor->GetMesh(), this->zMeshOffset);
		}
		else
		{
			this->zEng->GetCamera()->RemoveMesh();
			this->zCreated = false;
		}
		this->zActorType = this->zMsgHandler.ConvertStringToInt(M_ACTOR_TYPE, msgArray[1]);
	}
	else if(msgArray[0].find(M_CONNECTED.c_str()) == 0)
	{
		Vector3 camDir = this->zEng->GetCamera()->GetForward();
		Vector3 camUp = this->zEng->GetCamera()->GetUpVector();

		std::string serverMessage = "";
		serverMessage = this->zMsgHandler.Convert(MESSAGE_TYPE_USER_DATA);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_MESH_MODEL, this->zMeshID);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_DIRECTION, camDir);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_UP, camUp);
		serverMessage += this->zMsgHandler.Convert(MESSAGE_TYPE_USER_NAME, zName);

		this->zServerChannel->Send(serverMessage);

		this->zReady = false;
	}
	else if(msgArray[0].find(M_LOAD_MAP.c_str()) == 0)
	{
		// Server tells client to load a specific map
		std::string mapName = this->zMsgHandler.ConvertStringToSubstring(M_LOAD_MAP, msgArray[0]);
		this->InitGraphics(mapName);
	}
	else if(msgArray[0].find(M_ERROR_MESSAGE.c_str()) == 0)
	{
		std::string error_Message = this->zMsgHandler.ConvertStringToSubstring(M_ERROR_MESSAGE, msgArray[0], true);
		this->DisplayMessageToClient(error_Message, true);
	}
	else if(msgArray[0].find(M_SERVER_ANNOUNCEMENT.c_str()) == 0)
	{
		std::string error_Message = this->zMsgHandler.ConvertStringToSubstring(M_SERVER_ANNOUNCEMENT, msgArray[0], true);
		this->DisplayMessageToClient(error_Message, false);
	}
	else if(msgArray[0].find(M_SERVER_FULL.c_str()) == 0)
	{
		this->CloseConnection("Server is full");
	}
	else if(msgArray[0].find(M_KICKED.c_str()) == 0)
	{
		this->CloseConnection("You got kicked");
	}
	else if(msgArray[0].find(M_SERVER_SHUTDOWN.c_str()) == 0)
	{
		this->CloseConnection("Server Shutdown");
	}
	else if(msgArray[0].find(M_START_GAME.c_str()) == 0)
	{
		this->zGameStarted = true;
	}
	else
	{
		MaloW::Debug("C: Unknown Message Header Was sent from server " + msgArray[0] + " in HandleNetworkMessage");
		MaloW::Debug("C: " + msg);
	}
	this->zPerf->PostMeasure("Network Message Handling", 1);
}

bool Client::CheckHumanSpecificMessages(std::vector<std::string> msgArray)
{
	if (msgArray[0].find(M_LOOT_OBJECT_RESPONSE.c_str()) == 0)
	{
		unsigned int actorID = this->zMsgHandler.ConvertStringToInt(M_LOOT_OBJECT_RESPONSE, msgArray[0]);
		this->HandleDisplayLootData(msgArray, actorID);
	}
	else if(msgArray[0].find(M_EQUIP_ITEM.c_str()) == 0)
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
	else if (msgArray[0].find(M_ITEM_CRAFT) == 0)
	{
		if (msgArray.size() > 1)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ITEM_CRAFT, msgArray[0]);
			unsigned int stacks = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, msgArray[1]);

			this->HandleCraftItem(id, stacks);
		}
		
	}
	else if(msgArray[0].find(M_ITEM_USE.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
		this->HandleUseItem(id);
	}
	else if(msgArray[0].find(M_ITEM_FILL.c_str()) == 0)
	{
		if (msgArray.size() > 2)
		{
			unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ITEM_FILL, msgArray[0]);
			unsigned int currentUses = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_CURRENT, msgArray[1]);

			this->HandleFillItem(id, currentUses);
		}
	}
	else if(msgArray[0].find(M_REMOVE_EQUIPMENT.c_str()) == 0)
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
	else if(msgArray[0].find(M_UNEQUIP_ITEM.c_str()) == 0)
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
	else if(msgArray[0].find(M_ADD_INVENTORY_ITEM.c_str()) == 0)
	{
		this->HandleAddInventoryItem(msgArray);
	}
	else if(msgArray[0].find(M_REMOVE_INVENTORY_ITEM.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_REMOVE_INVENTORY_ITEM, msgArray[0]);
		this->HandleRemoveInventoryItem(id);
	}
	else if(msgArray[0].find(M_WEAPON_USE.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);
		this->HandleWeaponUse(id);
	}
	else
	{
		return false;
	}
	
	return true;
}

bool Client::HandleTakeDamage( const unsigned int ID, float damageTaken )
{
	Actor* actor = this->zActorManager->GetActor(ID);
	Actor* player = this->zActorManager->GetActor(this->zID);

	this->zHealth -= damageTaken;

	if(!player)
	{
		MaloW::Debug("Failed to find this Player in Client::HandleTakeDamage");
		return false;
	}
	if (!actor)
	{
		MaloW::Debug("Failed to find Attacker in Client::HandleTakeDamage");
		return false;
	}

	Vector3 actorPos = actor->GetPosition();
	Vector3 playerPos = player->GetPosition(); //Could using the camera's position here work better?
	Vector3 resultingVector = actorPos - playerPos;
	resultingVector.Normalize();

	if(this->zDamageIndicator == NULL && this->zHealth < 0.0f)
	{
		float windowHeight = (float)this->zEng->GetEngineParameters().WindowHeight;
		float windowWidth = (float)this->zEng->GetEngineParameters().WindowWidth;
		//Make the effect visible based on severity of attack.
		this->zDamageOpacity = damageTaken / 100;
		if(this->zDamageOpacity > 0.7f)
		{
			this->zDamageOpacity = 0.7f;
		}
		else if(this->zDamageOpacity < 0.25f)
		{
			this->zDamageOpacity = 0.25f;
		}

		if(ID == this->zID)
		{
			//There is no attacker, (except techincally the player)
			this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/behindOrFront_Temp.png" );
		}
		else
		{
			Vector3 properForward = this->zEng->GetCamera()->GetForward();
			properForward.y = 0.0f;
			properForward.Normalize();
			float testDotProduct = properForward.GetDotProduct(resultingVector);
			//float dotProduct = this->zEng->GetCamera()->GetForward().GetDotProduct( resultingVector ); 
		
			Vector3 rightVector = this->zEng->GetCamera()->GetForward().GetCrossProduct( this->zEng->GetCamera()->GetUpVector());
		
			float rightOrLeft = rightVector.GetDotProduct( resultingVector );
		
			//psuedo     rightOrLeft < 0 = höger,  rightOrLeft > 0 = vänster

			float upDotProduct = this->zEng->GetCamera()->GetUpVector().GetDotProduct( resultingVector );

			//psuedo    upDotProduct > 0 = upp,   upDotProduct < 0 = ner.

			if( testDotProduct > 0.65 && abs(upDotProduct) < 0.3 || testDotProduct < -0.7) 
			{
				//The damage is coming from the front and not too high or low. Or, it is coming from behind
				//Display surroundingpicture
				this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/behindOrFront_Temp.png" );
			}
			else //The damage is more specifically trackable.
			{
				if(testDotProduct < 0.65 && rightOrLeft > 0.0f)
				{
					if(upDotProduct > 0.3f) //It's up to the left
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/highLeft_Temp.png" );
					}
					else if(upDotProduct < -0.3f) //It's down to the left
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/lowLeft_Temp.png" );
					}
					else //It's directly to the left
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/left_Temp.png" );
					}		
				}
				else if(testDotProduct < 0.65 && rightOrLeft < 0.0f) //it's to the right (needs testing)
				{
					if(upDotProduct > 0.3f) //It's up to the right
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/highRight_Temp.png" );
					}
					else if(upDotProduct < -0.3f) //It's down to the right
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/lowRight_Temp.png" );
					}
					else //It's directly to the right
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/right_Temp.png" );
					}
				}
				else// if (dotProduct > 0.65)//It's in front, but too high or low
				{
					if(upDotProduct > 0.3f) //Up high.
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/up_Temp.png" );
					}
					else if(upDotProduct < -0.3f) //Down low.
					{
						this->zDamageIndicator = this->zEng->CreateImage(Vector2(), Vector2(windowWidth, windowHeight), "Media/Icons/down_Temp.png" );
					}
				}
			}
		}
		//Set the opacity
		this->zDamageIndicator->SetOpacity(this->zDamageOpacity);

	}
	return true;
}

void Client::UpdateHealthAndBleedingImage()
{
	this->zHealthOpacity = this->zHealth / 100;
	float goalOffset = 500.0f * this->zHealthOpacity;
	

	if(this->zCurrentOffset < goalOffset)
	{
		this->zCurrentOffset += 20.0f * zDeltaTime;
	}
	else if(this->zCurrentOffset > goalOffset)
	{
		this->zCurrentOffset -= 20.0f * zDeltaTime;
	}


	this->zHealthOpacity = 1 - this->zHealthOpacity;
	if(this->zHealthOpacity > 0.44f)
	{
		this->zHealthOpacity = 0.44f;
	}

	//this->zHealthOpacity = 0.5f;

		
	float windowHeight = (float)this->zEng->GetEngineParameters().WindowHeight;
	float windowWidth = (float)this->zEng->GetEngineParameters().WindowWidth;

	if(this->zBleedingAndHealthIndicator == NULL)
	{
		this->zBleedingAndHealthIndicator = this->zEng->CreateImage(Vector2(0 - this->zCurrentOffset,0 - this->zCurrentOffset), Vector2(windowWidth + this->zCurrentOffset*2, windowHeight + this->zCurrentOffset*2), "Media/Icons/HealthAndBleeding_Small_Temp.png" );
	}



	if(this->zBleedingLevel > 1)
	{
		if(!this->zDroppingPulse)
		{
			this->zBleedingOpacity += this->zDeltaTime * 0.14f * (this->zBleedingLevel - 1.0f);
		}
		else
		{
			this->zBleedingOpacity -= this->zDeltaTime * 0.14f * (this->zBleedingLevel - 1.0f);
		}


		if(this->zBleedingOpacity >= 0.22f || this->zBleedingOpacity <= 0.0f)
		{
			if(!this->zDroppingPulse)
			{
				this->zDroppingPulse = true;
			}
			else 
			{
				this->zDroppingPulse = false;
			}
		}
	}
	else
	{
		this->zBleedingOpacity = 0.0f;
	}
	//this->zHealthOpacity += testBleed;

	


	/*if(this->zPulseCounter > pulseLimit)
	{
		this->zPulseCounter = 0.0f;
		if(!this->zDroppingPulse)
		{
			this->zDroppingPulse = true;
		}
		else
		{
			this->zDroppingPulse = false;
		}
	}*/



	this->zBleedingAndHealthIndicator->SetOpacity(this->zHealthOpacity + this->zBleedingOpacity);
	this->zBleedingAndHealthIndicator->SetPosition(Vector2(0 - this->zCurrentOffset,0 - this->zCurrentOffset) );
	this->zBleedingAndHealthIndicator->SetDimensions(Vector2(windowWidth + this->zCurrentOffset*2, windowHeight + this->zCurrentOffset*2));


}

void Client::CloseConnection(const std::string& reason)
{
	//MaloW::Debug("Client Shutdown: " + reason);
	this->AddDisplayText("Client Shutdown: " + reason, false);
	//Todo Skriv ut vilket reason som gavs
	this->zServerChannel->TrySend(this->zMsgHandler.Convert(MESSAGE_TYPE_CONNECTION_CLOSED, (float)this->zID));
	Sleep(2000);
	this->zServerChannel->Close();
	this->Close();
}

std::vector<unsigned int> Client::RayVsWorld()
{
	static const float MAX_DISTANCE_TO_OBJECT = 3.0f;

	Vector3 origin = this->zEng->GetCamera()->GetPosition();
	Vector3 camForward = this->zEng->GetCamera()->GetForward();

	CollisionData data;
	std::vector<unsigned int> Collisions;
	//Static objects
	std::map<unsigned int, Actor*> actors = this->zActorManager->GetActors();
	iMesh* mesh = NULL;
	auto it_actors_end = actors.end();
	for(auto it = actors.begin(); it != it_actors_end; it++)
	{
		Actor* actor = it->second;
		unsigned int ID = it->first;
		if (ID != this->zID)
		{
			mesh = actor->GetMesh();
			if (!mesh)
			{
				MaloW::Debug("ERROR: Mesh is Null in RayVsWorld function");
				continue;
			}
			if (!dynamic_cast<iFBXMesh*>(mesh))
			{
				data = this->zEng->GetPhysicsEngine()->GetCollisionRayMeshBoundingOnly(origin, camForward, mesh);

				if (data.collision && data.distance < MAX_DISTANCE_TO_OBJECT)
				{
					Collisions.push_back(ID);
				}
			}
			
		}
	}

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

void Client::DisplayMessageToClient(const std::string& msg, bool bError)
{
	this->AddDisplayText(msg, bError);

	//MaloW::Debug(msg);
}

void Client::OnEvent(Event* e)
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

void Client::HandleDisplayLootData(std::vector<std::string> msgArray, const unsigned int ActorID)
{
	Gui_Item_Data gid;
	auto it_string_end = msgArray.end();
	for (auto it_Item_Data = msgArray.begin() + 1; it_Item_Data != it_string_end; it_Item_Data++)
	{
		if((*it_Item_Data).find(M_OBJECT_ID.c_str()) == 0)
		{
			gid.zID = this->zMsgHandler.ConvertStringToInt(M_OBJECT_ID, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_TYPE.c_str()) == 0)
		{
			gid.zType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_SUB_TYPE.c_str()) == 0)
		{
			gid.zSubType = this->zMsgHandler.ConvertStringToInt(M_ITEM_SUB_TYPE, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			gid.zDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it_Item_Data), true);
		}
		else if((*it_Item_Data).find(M_ITEM_NAME.c_str()) == 0)
		{
			gid.zName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it_Item_Data), true);
		}
		else if((*it_Item_Data).find(M_ITEM_WEIGHT.c_str()) == 0)
		{
			gid.zWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_STACK_SIZE.c_str()) == 0)
		{
			gid.zStacks = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_ICON_PATH.c_str()) == 0)
		{
			gid.zFilePath = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_FINISHED.c_str()) == 0)
		{
			this->zGuiManager->AddLootItemToLootGui(gid);
		}
	}
	if(this->zGuiManager->IsCraftOpen())
		this->zGuiManager->ToggleCraftingGui();
	this->zGuiManager->ToggleInventoryGui();
	this->zGuiManager->ToggleLootGui(ActorID);
	this->zShowCursor = true;
	//this->SendLootItemMessage(ActorID, gid.zID, gid.zType, gid.zSubType);
}

void Client::UpdateCameraOffset(unsigned int state)
{
	iMesh* mesh = NULL;
	Vector3 position = this->zEng->GetCamera()->GetPosition();
	if (Actor* actor = this->zActorManager->GetActor(this->zID))
	{
		mesh = actor->GetMesh();
	}
	
	if (state == STATE_CROUCHING)
	{
		auto cameraPos = this->zStateCameraOffset.find(state);

		Vector3 offset = cameraPos->second;

		//this->zEng->GetCamera()->SetMesh(mesh, offset, Vector3(0.0f, 0.0f, 1.0f));
		//this->zActorManager->SetCameraOffset(offset);
	}
	else
	{
		this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset, Vector3(0.0f, 0.0f, 1.0f));
		this->zActorManager->SetCameraOffset(this->zMeshOffset);
	}
}

void Client::UpdateText()
{
	Vector2 position;
	auto it = this->zDisplayedText.begin();
	while (it != this->zDisplayedText.end())
	{
		TextDisplay* text  = (*it);

		text->zTimer -= this->zDeltaTime;

		if (text->zTimer <= 0)
		{
			position = text->zText->GetPosition();

			this->zEng->DeleteText(text->zText);

			it = this->zDisplayedText.erase(it);

			auto it_text_end = this->zDisplayedText.end();
			for (auto new_It = it; new_It != it_text_end; new_It++)
			{
				Vector2 oldPos = (*new_It)->zText->GetPosition();

				(*new_It)->zText->SetPosition(position);

				position = oldPos;
			}
			
			delete text;
			text = NULL;
		}
		else
		{
			it++;
		}
	}	
}

void Client::AddDisplayText(const std::string& msg, bool bError)
{
	static const float START_TEXT_TIMER = 7.0f;

	std::string newString = msg;

	std::replace(newString.begin(), newString.end(), '_', ' ');

	int arrSize = (int)this->zDisplayedText.size();
	int windowHeight = this->zEng->GetEngineParameters().WindowHeight;

	float yStartPosition = 80.0f;
	float xPosition = 50.0f;
	float textheight = 20.0f;

	static const int MAX_ITEMS = (int)( (windowHeight - yStartPosition) * 0.05f );

	Vector2 position;

	float c = 0;
	
	if (arrSize >= MAX_ITEMS)
	{
		TextDisplay* temp = this->zDisplayedText[0];
		this->zDisplayedText.erase(this->zDisplayedText.begin());

		this->zEng->DeleteText(temp->zText);
		SAFE_DELETE(temp);
	}
	auto it_text_end = this->zDisplayedText.end();
	for (auto it = this->zDisplayedText.begin(); it != it_text_end; it++)
	{
		float x = (*it)->zText->GetPosition().x;
		position = Vector2(x, yStartPosition + c++ * textheight);
		(*it)->zText->SetPosition(position);

		
	}

	if (arrSize == 0)
	{
		position = Vector2(xPosition, yStartPosition);
	}
	else
	{
		position = Vector2(xPosition, yStartPosition + c++ * textheight);
	}
	iText* text = NULL;

	text = this->zEng->CreateText(newString.c_str(), position, 0.7f, "Media/Fonts/new");
	
	if (bError)
		text->SetColor(Vector3(0.0f, -255.0f, -255.0f));
	else
		text->SetColor(Vector3(0.0f, 0.0f, -255.0f));

	TextDisplay* displayedText = new TextDisplay(text, START_TEXT_TIMER, bError);

	this->zDisplayedText.push_back(displayedText);
}

void Client::UpdatePhysicalCondition( PhysicalConditionPacket* PCP )
{
	if(PCP->zHealth != -1.0f)
		this->zHealth = PCP->zHealth;

	if(PCP->zEnergy != -1.0f)
		this->zEnergy = PCP->zEnergy;

	if(PCP->zBleedingLevel != -1.0f)
		this->zBleedingLevel = PCP->zBleedingLevel;

	if(PCP->zHunger != -1.0f)
		this->zHunger = PCP->zHunger;

	if(PCP->zHydration != -1.0f)
		this->zHydration = PCP->zHydration;

	if(PCP->zStamina != -1.0f)
		this->zStamina = PCP->zStamina;
}

void Client::ResetPhysicalConditions()
{
	this->zHealth = 100;
	this->zEnergy = zEnergy;
	this->zBleedingLevel = 0;
	this->zHunger = 100;
	this->zHydration = 100;
	this->zStamina = 100;
}