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

using namespace MaloW;

// Timeout_value = 10 sek
static const float TIMEOUT_VALUE = 10.0f;

// 50 updates per second
static const float UPDATE_DELAY = 0.020f;
static const float MAX_DISTANCE_TO_OBJECT = 3.0f;

static const float START_TEXT_TIMER = 7.0f;

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
	this->zMeshCameraOffsets["Media/Models/Ball.obj"] = Vector3();
	this->zMeshCameraOffsets["Media/Models/temp_guy.obj"] = Vector3(0.0f, 1.9f, 0.0f);
	this->zMeshCameraOffsets["Media/Models/deer_temp.obj"] = Vector3(0.0f, 1.7f, 0.0f);
	this->zMeshCameraOffsets["Media/Models/temp_guy_movement_anims.fbx"] = Vector3(0.0f, 2.5f, 0.0f);

	this->zStateCameraOffset[STATE_IDLE] = Vector3(0.0f, 0.0f, 0.0f);
	this->zStateCameraOffset[STATE_RUNNING] = Vector3(0.0f, 0.0f, 0.0f);
	this->zStateCameraOffset[STATE_WALKING] = Vector3(0.0f, 0.0f, 0.0f);
	this->zStateCameraOffset[STATE_CROUCHING] = Vector3(0.0f, 1.0f, 0.0f);

	this->zAnimationFileReader[0] = AnimationFileReader("Media/Models/temp_guy_movement_anims.txt");
	this->zAnimationFileReader[0].ReadFromFile();

	this->zModelToReaderMap["Media/Models/temp_guy_movement_anims.fbx"] = zAnimationFileReader[0];

	this->zSendUpdateDelayTimer = 0.0f;

	this->zEng = GetGraphics();
	
	this->zGuiManager = NULL;
	this->zActorManager = NULL;
	this->zServerChannel = NULL;
	this->zPlayerInventory = NULL;
	this->zKeyInfo = KeyHandler();
	this->zKeyInfo.InitKeyBinds();
	
	this->zMsgHandler = NetworkMessageConverter();

	this->zUps;
	this->zLatencyText = NULL;
	this->zUpsText = NULL;
	this->zWorld = NULL;
	this->zWorldRenderer = NULL;
	this->zAnchor = NULL;
	this->zCrossHair = NULL;
	this->zDamageIndicator = NULL;
	this->zDamageOpacity = 0.0f;
	
	this->zIgm = new InGameMenu();
	this->zPam = new PickAnimalMenu();
}

void Client::Connect(const std::string &IPAddress, const unsigned int &port, std::string& errMsg, int& errorCode)
{
	this->zIP = IPAddress;
	this->zPort = port;
	this->zServerChannel = new ServerChannel(this, IPAddress, port, errMsg, errorCode);
	if (errMsg == "")
		this->zServerChannel->Start();
}

Client::~Client()
{
	this->zEng->GetCamera()->RemoveMesh();

	this->Close();
	this->WaitUntillDone();

	if (this->zAnchor)
		this->zWorld->DeleteAnchor(this->zAnchor);

	SAFE_DELETE(this->zGuiManager);
	SAFE_DELETE(this->zActorManager);
	SAFE_DELETE(this->zServerChannel);
	SAFE_DELETE(this->zPlayerInventory);

	SAFE_DELETE(this->zIgm);
	SAFE_DELETE(this->zPam);

	SAFE_DELETE(this->zWorldRenderer);
	SAFE_DELETE(this->zWorld);
	
	this->zMeshCameraOffsets.clear();
	this->zStateCameraOffset.clear();

	if (this->zBlackImage)
		this->zEng->DeleteImage(this->zBlackImage);
	
	if (this->zDamageIndicator)
		this->zEng->DeleteImage(this->zDamageIndicator);

	if (this->zCrossHair) 
		this->zEng->DeleteImage(this->zCrossHair);
	
	if (this->zUpsText)
		this->zEng->DeleteText(this->zUpsText);


	for (auto it = this->zDisplayedText.begin(); it != this->zDisplayedText.end(); it++)
	{
		TextDisplay* temp = (*it);

		this->zEng->DeleteText(temp->zText);
		SAFE_DELETE(temp);
	}

	this->zDisplayedText.clear();
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

	if (this->zGuiManager)
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
		if ( zWorldRenderer ) 
			zWorldRenderer->Update();

		this->IgnoreRender( 50.0f, zEng->GetCamera()->GetPosition().GetXZ() );
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
	return this->zDeltaTime;
}

void Client::IgnoreRender( const float& radius, Vector2& center )
{
	static std::set<Entity*> previousEntities;
	std::set<Entity*> entities; 
	std::set<Entity*> validEntities;

	zWorld->GetEntitiesInCircle(center, radius, entities);

	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		if ( GetEntBlockRadius( (*it)->GetType() ) <= 0.0f )
		{
			iMesh* mesh = this->zWorldRenderer->GetEntityMesh(*it);
			
			if(mesh)
			{
				mesh->DontRender(false);
				validEntities.insert(*it);
			}
		}
	}

	/*Check previous, if prev is not in valid, they should not be rendered.*/
	for(auto it = previousEntities.begin(); it != previousEntities.end(); it++)
	{
		auto found = validEntities.find(*it);

		if(found == validEntities.end())
		{
			this->zWorldRenderer->GetEntityMesh(*it)->DontRender(true);
		}
	}

	previousEntities.clear();
	previousEntities = validEntities;
}

void Client::InitGraphics(const std::string& mapName)
{
	if (this->zActorManager)
		delete this->zActorManager;

	if (this->zPlayerInventory)
		delete this->zPlayerInventory;

	if (this->zGuiManager)
		delete this->zGuiManager;

	this->zActorManager = new ClientActorManager();
	this->zActorManager->SetFBXMapping(this->zModelToReaderMap);
	this->zGuiManager = new GuiManager(this->zEng);
	this->zPlayerInventory = new Inventory();

	LoadEntList("Entities.txt");

	if ( zWorld ) 
		delete zWorld, zWorld=0;

	this->zWorld = new World(this, mapName, true);

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
	this->zWorldRenderer->Update();
	
	this->zEng->DeleteImage(this->zBlackImage);
	this->zBlackImage = NULL;

	this->zEng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 0.0f, 1.0f, 0.2f, 0.2f);	//this->zEng->StartRendering();
	
	if (this->zCrossHair)
		this->zEng->DeleteImage(this->zCrossHair);

	this->zCrossHair = this->zEng->CreateImage(Vector2(xPos, yPos), Vector2(length, length), "Media/Icons/cross.png");
	this->zCrossHair->SetOpacity(0.5f);

	if (this->zUpsText)
		this->zEng->DeleteText(this->zUpsText);

	this->zUpsText = this->zEng->CreateText("", Vector2(1, 1), 0.7f, "Media/Fonts/1");

	if (this->zLatencyText)
		this->zEng->DeleteText(this->zLatencyText);

	this->zLatencyText = this->zEng->CreateText("0 ms", Vector2(1, 20), 0.7f, "Media/Fonts/1");;

	//Go through entities (bush etc) and set render flag.
	std::set<Entity*> entities;
	Vector2 size = zWorld->GetWorldSize();
	float radius = powf(size.x, 2.0f) + powf(size.y, 2.0f);
	zWorld->GetEntitiesInCircle(center, (radius * 0.5f), entities);

	for (auto i = entities.begin(); i != entities.end(); i++)
	{
		if ( GetEntBlockRadius( (*i)->GetType() ) <= 0.0f )
		{
			iMesh* mesh = this->zWorldRenderer->GetEntityMesh(*i);
			mesh->DontRender(true);
		}
	}

}

void Client::Init()
{
	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);
}

void Client::Life()
{
	MaloW::Debug("Client Process Started");

	float counter = 0.0f;
	
	this->Init();
	while(this->zEng->IsRunning() && this->stayAlive)
	{
		this->Update();
		this->UpdateText();
		this->CheckKeyboardInput();
		if(this->zCreated)
		{
			this->zSendUpdateDelayTimer += this->zDeltaTime;
			this->zTimeSinceLastPing += this->zDeltaTime;

			zUps++;
			counter += this->zDeltaTime;
			if (counter >= 1.0f)
			{
				std::string text = MaloW::convertNrToString((float)this->zUps);
				this->zUpsText->SetText(text.c_str());
				this->zUps = 0;
				counter = 0;
			}

 			if(this->zSendUpdateDelayTimer >= UPDATE_DELAY)
 			{
				this->zSendUpdateDelayTimer = 0.0f;

				this->SendClientUpdate();
			}

			this->UpdateMeshRotation(); // Will manage the local model later.

			this->UpdateActors();
		}

		AudioManager* am = AudioManager::GetInstance();
		am->Update();

		this->ReadMessages();
		if(this->zPam->GetShow())
		{
			int returnValue = this->zPam->Run();
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
				CloseConnection("Disconnected");
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

void Client::UpdateActors()
{
	this->zActorManager->UpdateObjects(this->zDeltaTime, this->zID);
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
	this->CheckKey(KEY_FORWARD);
	this->CheckKey(KEY_BACKWARD);

	this->CheckKey(KEY_LEFT);
	this->CheckKey(KEY_RIGHT);
}

void Client::CheckPlayerSpecificKeys()
{
	if(this->zGuiManager->IsGuiOpen())
	{
		Menu_select_data msd;
		msd = this->zGuiManager->CheckCollisionInv(); // Returns -1 on both values if no hits.

		if (msd.zAction != -1)
		{
			if (msd.zID != -1)
			{
				Item* item = this->zPlayerInventory->SearchAndGetItem(msd.zID);
				if (msd.zAction == USE)
				{
					if (item)
						SendUseItemMessage(item->GetID());
				}
				if (msd.zAction == CRAFT)
				{
					if (item)
						SendCraftItemMessage(item->GetID());
				}
				else if(msd.zAction == EQUIP)
				{
					if(item)
						SendEquipItem(msd.zID);
				}
				else if (msd.zAction == DROP)
				{
					if(item)
						this->SendDropItemMessage(msd.zID);
				}
				else if (msd.zAction == UNEQUIP)
				{
					if(item)
						this->SendUnEquipItem(msd.zID, (msd.zType));
				}
			}
		}
	}

	if(this->zEng->GetKeyListener()->IsPressed(this->zKeyInfo.GetKey(KEY_INTERACT)))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_INTERACT))
		{
			std::vector<unsigned int> collisionObjects = this->RayVsWorld();
			if (collisionObjects.size() > 0)
			{
				std::string msg =  "";
				for (auto it = collisionObjects.begin(); it != collisionObjects.end(); it++)
				{
					msg += this->zMsgHandler.Convert(MESSAGE_TYPE_LOOT_OBJECT, (float)(*it));
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
			this->zGuiManager->HideLootingGui();
			this->zKeyInfo.SetKeyState(KEY_INTERACT, false);
		}
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
			this->zShowCursor = !this->zShowCursor;
			this->zGuiManager->ToggleInventoryGui();
		}
	}
	else
	{
		if (this->zKeyInfo.GetKeyState(KEY_INVENTORY))
			this->zKeyInfo.SetKeyState(KEY_INVENTORY, false);
	}

	if (!this->zGuiManager->IsGuiOpen() && !zShowCursor)
	{
		if (this->zEng->GetKeyListener()->IsClicked(1))
		{
			if (!this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
			{
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, true);

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
		else
		{
			if (this->zKeyInfo.GetKeyState(MOUSE_LEFT_PRESS))
				this->zKeyInfo.SetKeyState(MOUSE_LEFT_PRESS, false);
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
	//Sound debug
	else if (this->zEng->GetKeyListener()->IsPressed(VK_F8))
	{
		if (!this->zKeyInfo.GetKeyState(KEY_DEBUG_INFO))
		{
			std::stringstream ss;
			Vector3 position = this->zEng->GetCamera()->GetPosition();
			Vector3 direction = this->zEng->GetCamera()->GetForward();
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
			ss << "Terrain Normal Error at " << std::endl;
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

void Client::HandleNetworkPacket( Packet* P )
{
	if ( ServerFramePacket* SFP = dynamic_cast<ServerFramePacket*>(P) )
	{
		this->UpdateActors(SFP);	
	}
	else if (NewActorPacket* NPA = dynamic_cast<NewActorPacket*>(P))
	{
		this->AddActor(NPA);
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

		if ( !packet ) 
			throw("Unknown Packet Type");

		// Deserialize
		packet->Deserialize(ss);

		// Handle
		HandleNetworkPacket(packet);

		return;
	}

	std::vector<std::string> msgArray;
	msgArray = this->zMsgHandler.SplitMessage(msg);

	if (Messages::MsgFileWrite())
		Messages::Debug(msg);

	//Checks what type of message was sent
	if(msgArray[0].find(M_PING.c_str()) == 0)
	{
		this->Ping();
	}
	else if (msgArray[0].find(M_HEALTH) == 0)
	{
	}
	else if (msgArray[0].find(M_CLIENT_LATENCY.c_str()) == 0)
	{
		float latency = this->zMsgHandler.ConvertStringToFloat(M_CLIENT_LATENCY, msgArray[0]);
		latency *= 1000.0f;

		int ms = (int)latency;
		std::string text = MaloW::convertNrToString((float)ms) + " ms";

		zLatencyText->SetText(text.c_str());
	}
	else if (msgArray[0].find(M_PLAY_ANIMATION.c_str()) == 0)
	{
		if (this->zReady)
		{
			std::string animationName = this->zMsgHandler.ConvertStringToSubstring(M_PLAY_ANIMATION, msgArray[0]);
			unsigned int actorID = this->zMsgHandler.ConvertStringToInt(M_OBJECT_ID, msgArray[1]);

			Actor* actor = this->zActorManager->GetActor(actorID);
			if (actor)
			{
				iFBXMesh* mesh = dynamic_cast<iFBXMesh*>(actor->GetMesh());
				if (mesh)
				{
					std::string model = actor->GetModel();
					auto it = this->zModelToReaderMap.find(model);
					if (it != this->zModelToReaderMap.end())
					{
						std::string animation = it->second.GetAnimation(animationName);

						if (animation != "")
							mesh->SetAnimation(animation.c_str());
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
		std	::string fileName = this->zMsgHandler.ConvertStringToSubstring(M_PLAY_SOUND, msgArray[0]);
		Vector3 pos = this->zMsgHandler.ConvertStringToVector(M_POSITION, msgArray[1]);

	}
	else if(msgArray[0].find(M_SELF_ID.c_str()) == 0)
	{
		this->zID = this->zMsgHandler.ConvertStringToInt(M_SELF_ID, msgArray[0]);

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
		MaloW::Debug("C: Unknown Message Was sent from server " + msgArray[0] + " in HandleNetworkMessage");
		MaloW::Debug("C: " + msg);
	}
}

bool Client::CheckHumanSpecificMessages(std::vector<std::string> msgArray)
{
	if (msgArray[0].find(M_LOOT_OBJECT_RESPONSE.c_str()) == 0)
	{
		this->HandleDisplayLootData(msgArray);
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
	else if(msgArray[0].find(M_ITEM_USE.c_str()) == 0)
	{
		unsigned int id = this->zMsgHandler.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
		this->HandleUseItem(id);
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

	if (!actor && !player)
	{
		MaloW::Debug("Failed to find Attacker in Client::HandleTakeDamage");
		return false;
	}

	Vector3 actorPos = actor->GetPosition();
	Vector3 playerPos = player->GetPosition(); //Could using the camera's position here work better?
	Vector3 resultingVector = actorPos - playerPos;
	resultingVector.Normalize();

	if(this->zDamageIndicator == NULL)
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

void Client::CloseConnection(const std::string& reason)
{
	MaloW::Debug("Client Shutdown: " + reason);
	//Todo Skriv ut vilket reason som gavs
	this->zServerChannel->TrySend(this->zMsgHandler.Convert(MESSAGE_TYPE_CONNECTION_CLOSED, (float)this->zID));
	this->zServerChannel->Close();
	this->Close();
}

std::vector<unsigned int> Client::RayVsWorld()
{
	Vector3 origin = this->zEng->GetCamera()->GetPosition();
	Vector3 camForward = this->zEng->GetCamera()->GetForward();

	CollisionData data;
	std::vector<unsigned int> Collisions;
	//Static objects
	std::map<unsigned int, Actor*> actors = this->zActorManager->GetActors();
	iMesh* mesh = NULL;
	for(auto it = actors.begin(); it != actors.end(); it++)
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

			data = this->zEng->GetPhysicsEngine()->GetCollisionRayMeshBoundingOnly(origin, camForward, mesh);

			if (data.collision && data.distance < MAX_DISTANCE_TO_OBJECT)
			{
				Collisions.push_back(ID);
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

	MaloW::Debug(msg);
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

void Client::HandleDisplayLootData(std::vector<std::string> msgArray)
{
	std::vector<Looting_Gui_Data> guiData;
	Looting_Gui_Data lgd = Looting_Gui_Data();
	for (auto it_Item_Data = msgArray.begin() + 1; it_Item_Data != msgArray.end(); it_Item_Data++)
	{
		if((*it_Item_Data).find(M_OBJECT_ID.c_str()) == 0)
		{
			lgd.zGui_Data.zID = this->zMsgHandler.ConvertStringToInt(M_OBJECT_ID, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_TYPE.c_str()) == 0)
		{
			lgd.zGui_Data.zType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_SUB_TYPE.c_str()) == 0)
		{
			lgd.zGui_Data.zSubType = this->zMsgHandler.ConvertStringToInt(M_ITEM_SUB_TYPE, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			lgd.zGui_Data.zDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it_Item_Data), true);
		}
		else if((*it_Item_Data).find(M_ITEM_NAME.c_str()) == 0)
		{
			lgd.zGui_Data.zName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it_Item_Data), true);
		}
		else if((*it_Item_Data).find(M_ITEM_WEIGHT.c_str()) == 0)
		{
			lgd.zGui_Data.zWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_STACK_SIZE.c_str()) == 0)
		{
			lgd.zGui_Data.zStacks = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_ICON_PATH.c_str()) == 0)
		{
			lgd.zGui_Data.zFilePath = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it_Item_Data));
		}
		else if((*it_Item_Data).find(M_ITEM_FINISHED.c_str()) == 0)
		{
			lgd.zActorID = this->zMsgHandler.ConvertStringToInt(M_ITEM_FINISHED, (*it_Item_Data));
			
			guiData.push_back(lgd);
		}
	}

	this->SendLootItemMessage(guiData[0].zActorID, guiData[0].zGui_Data.zID, guiData[0].zGui_Data.zType, guiData[0].zGui_Data.zSubType);
	this->zGuiManager->ShowLootingGui(guiData);
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

		this->zEng->GetCamera()->SetMesh(mesh, offset, Vector3(0.0f, 0.0f, 1.0f));
		this->zActorManager->SetCameraOffset(offset);
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

			for (auto new_It = it; new_It != this->zDisplayedText.end(); new_It++)
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
	std::string newString = msg;

	std::replace(newString.begin(), newString.end(), '_', ' ');

	int arrSize = (int)this->zDisplayedText.size();
	int windowWidth = this->zEng->GetEngineParameters().WindowWidth;
	int windowHeight = this->zEng->GetEngineParameters().WindowHeight;

	float yStartPosition = 60.0f;
	float xPosition = 50.0f;
	float textheight = 20.0f;

	static const int MAX_ITEMS = (windowHeight - yStartPosition) * 0.05f;

	Vector2 position;

	float c = 0;
	
	if (arrSize >= MAX_ITEMS)
	{
		TextDisplay* temp = this->zDisplayedText[0];
		this->zDisplayedText.erase(this->zDisplayedText.begin());

		this->zEng->DeleteText(temp->zText);
		SAFE_DELETE(temp);
	}

	for (auto it = this->zDisplayedText.begin(); it != this->zDisplayedText.end(); it++)
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
	std::string fontPath = "";
	if (bError)
		fontPath = "Media/Fonts/1";
	else
		fontPath = "Media/Fonts/3";

	iText* text = this->zEng->CreateText(newString.c_str(), position, 0.7f, fontPath.c_str());

	TextDisplay* displayedText = new TextDisplay(text, START_TEXT_TIMER);

	this->zDisplayedText.push_back(displayedText);
}