#include "Host.h"
#include "ClientServerMessages.h"
#include "ClientConnectedEvent.h"
#include "ClientDisconnectedEvent.h"

// 50 updates per sec
static const float UPDATE_DELAY = 0.020f;

Host::Host()
{
	MaloW::ClearDebug();
	Messages::ClearDebug();
#ifdef INCLUDE_MODEL_VIEWER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MaloW::Debug("(DEBUG): Server: Debug flag set to: _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF). ");
	MaloW::Debug("(DEBUG): Server: vld.h included.");
#endif

	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zMinClients = 0;
	this->zClients = std::vector<ClientData*>(); 

	this->zStartime = 0;
	this->zSecsPerCnt = 0.0f;
	this->zDeltaTime = 0.0f;
	this->zTimeOut = 15.0f;
	this->zPingMessageInterval = 5.0f;

	this->zWorld = 0;
	zAnchorPlayerMap.clear();
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();
	
	this->Close();
	this->WaitUntillDone();

	this->zServerListener->Close();
	SAFE_DELETE(this->zServerListener);

	for(auto x = zClients.begin(); x < zClients.end(); x++)
	{
		SAFE_DELETE(*x);
	}

	SAFE_DELETE(this->zWorld);
}

//NEEDS FIXING
void Host::Init()
{
	this->zGameMode->InitiGameObjects();}

void Host::Life()
{
	if (Messages::FileWrite())
		Messages::Debug("Host Process Started");

	this->zServerListener->Start();
	
	this->Init();

	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	
	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);

	static float waitTimer = 0.0f;
	
	this->zGameStarted = false;
	while(this->stayAlive)
	{
		if (zWorld) zWorld->Update();

		Update();

		//HandleClientNackIM();

		ReadMessages();
		if(this->zGameStarted)
		{
			waitTimer += this->zDeltaTime;
			PingClients();
			UpdateObjects();

			if(waitTimer >= UPDATE_DELAY)
			{
				waitTimer = 0.0f;

				SendPlayerActorUpdates();
				SendAnimalActorUpdates();
				SendDynamicActorUpdates();
			}
		}
		else
		{
			int clientsReady = 0;
			for(int i = 0; i < this->zClients.size(); i++)
			{
				if(this->zClients.at(i)->GetReady()) clientsReady++;
			}
			if(clientsReady >= this->zClients.size() && this->zClients.size())
			{
				this->zGameStarted = true;
				this->SendStartMessage();
			}
			/*if (this->zClients.size() >= this->zMinClients)
			{
				this->zGameStarted = true;
				this->SendStartMessage();
			}*/
		}
	
		Sleep(5);
	}
}

Vector3 Host::CalculateSpawnPoint(int currentPoint, int maxPoints, float radius, Vector3 center)
{
	static const float PI = 3.14159265358979323846f;

	float slice  = 2 * PI / maxPoints;

	float angle = slice * currentPoint;

	float x = center.x + radius * cos(angle);
	float z = center.z + radius * sin(angle);
	float y = 0.0f;

	if ( x >= 0.0f && y >= 0.0f && x<zWorld->GetWorldSize().x && y<zWorld->GetWorldSize().y )
	{
		y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));
	}

	return Vector3(x, y, z);
}

const char* Host::InitHost(const unsigned int &port, const unsigned int &maxClients)
{
	if ( zWorld ) delete zWorld, zWorld=0;
	this->zWorld = new World(this, "3x3.map");

	this->zGameMode = new GameModeFFA(this->zWorld);
	//this->zActorHandler = new ActorHandler(this->zWorld);
	try
	{
		if ( zServerListener ) delete zServerListener;
		zServerListener = new ServerListener(this, port);
		zServerListener->Start();
	}
	catch(const char *str)
	{
		return str;
	}

	return 0;
}

void Host::HandleNewConnection( MaloW::ClientChannel* CC )
{
	if (Messages::FileWrite())
		Messages::Debug("New Player Connected.");

	if((unsigned int)this->zClients.size() > zMaxClients)
	{
		std::string message;
		
		message = zMessageConverter.Convert(MESSAGE_TYPE_SERVER_FULL);

		CC->Send(message);
		SAFE_DELETE(CC);

		return;
	}

	ActorHandler* actorHandler = this->zGameMode->GetActorHandlerPtr();
	if (Messages::FileWrite())
		Messages::Debug("New Player Accepted.");

	std::string message = "";
	MaloW::ClientChannel* client = CC;
	ClientData* cd = new ClientData(client);
	this->zClients.push_back(cd);

	message = this->zMessageConverter.Convert(MESSAGE_TYPE_SELF_ID, (float)client->GetClientID());
	client->Start();
	SendToClient(cd, message, true);

	//Collect player infos
	message = "";
	std::vector<std::string> temp;
	std::vector<PlayerActor *> players = actorHandler->GetPlayers();

	//Gets Players Information
	for(auto it = players.begin(); it < players.end(); it++)
	{
		message =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (float)(*it)->GetID());
		message += (*it)->ToMessageString(&this->zMessageConverter);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());

		temp.push_back(message);
	}
	message = "";
	std::vector<AnimalActor *> animals = actorHandler->GetAnimals();

	//Gets AnimalInformation
	for(auto it = animals.begin(); it < animals.end(); it++)
	{
		message =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_ANIMAL, (float)(*it)->GetID());
		message += (*it)->ToMessageString(&this->zMessageConverter);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());

		temp.push_back(message);
	}

	this->GetExistingObjects(temp);

	std::vector<DeadPlayerObjectActor*> deadPlayers = actorHandler->GetDeadPlayers();
	std::vector<Item*> items;

	for (auto it = deadPlayers.begin(); it < deadPlayers.end(); it++)
	{
		message = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_DEAD_PLAYER_OBJECT, (float)(*it)->GetID());
		message += (*it)->ToMessageString(&this->zMessageConverter);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());

		items = (*it)->GetItems();
		for (auto x = items.begin(); x < items.end(); x++)
		{
			int item_type = (*x)->GetItemType();
			if (item_type == ITEM_TYPE_CONTAINER_CANTEEN || item_type == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
			{
				Container* container = dynamic_cast<Container*>((*x));

				if (!container)
				{
					MaloW::Debug("Failed to cast Material in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)container->GetID());

				message += container->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if (item_type == ITEM_TYPE_MATERIAL_SMALL_STICK || item_type == ITEM_TYPE_MATERIAL_MEDIUM_STICK ||
				item_type == ITEM_TYPE_MATERIAL_LARGE_STICK || item_type == ITEM_TYPE_MATERIAL_THREAD)
			{
				Material* material = dynamic_cast<Material*>((*x));

				if (!material)
				{
					MaloW::Debug("Failed to cast Material in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)material->GetID());

				message += material->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if (item_type == ITEM_TYPE_FOOD_DEER_MEAT || item_type == ITEM_TYPE_FOOD_WOLF_MEAT)
			{
				Food* food = dynamic_cast<Food*>((*x));

				if (!food)
				{
					MaloW::Debug("Failed to cast Food in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)food->GetID());

				message += food->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if (item_type == ITEM_TYPE_PROJECTILE_ARROW)
			{
				Projectile* projectile = dynamic_cast<Projectile*>((*x));

				if (!projectile)
				{
					MaloW::Debug("Failed to cast Projectile in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)projectile->GetID());

				message += projectile->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if (item_type == ITEM_TYPE_WEAPON_RANGED_BOW || item_type == ITEM_TYPE_WEAPON_RANGED_ROCK)
			{
				RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>((*x));

				if (!rWpn)
				{
					MaloW::Debug("Failed to cast Ranged Weapon in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)rWpn->GetID());

				message += rWpn->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if (item_type == ITEM_TYPE_WEAPON_MELEE_AXE || item_type == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
			{
				MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>((*x));

				if (!mWpn)
				{
					MaloW::Debug("Failed to cast Melee Weapon in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)mWpn->GetID());

				message += mWpn->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
		}
		temp.push_back(message);

	}

	//Dead animals
	std::vector<DeadAnimalObjectActor*> deadAnimals = actorHandler->GetDeadAnimals();
	for (auto it = deadAnimals.begin(); it < deadAnimals.end(); it++)
	{
		message = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_DEAD_ANIMAL_OBJECT, (float)(*it)->GetID());
		message += (*it)->ToMessageString(&this->zMessageConverter);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		
		std::vector<Item*> dead_animal_drop = (*it)->GetItems();
		Food* food;
		for (auto it_items = dead_animal_drop.begin(); it_items < dead_animal_drop.end(); it_items++)
		{
			food = dynamic_cast<Food*>((*it_items));
			
			message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_ANIMAL_ADD_ITEM, (float)food->GetID());
			
			message += food->ToMessageString(&this->zMessageConverter);

			message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_ANIMAL_ITEM_FINISHED);
		}
		temp.push_back(message);
	}

	//Sends All Actors to the player
	for (auto it = temp.begin(); it < temp.end(); it++)
	{
		SendToClient(cd,*it, true);
	}

}

void Host::SendToAllClients(const std::string& message, bool sendIM /*= false*/)
{
	if(!HasClients())
		return;

	std::string im;
	for(auto it = zClients.begin(); it < zClients.end(); it++)
	{
		/*
		if(sendIM)
		{
			unsigned long m_id = (*it)->GetNextIPID();
			im = this->zMessageConverter.Convert(MESSAGE_TYPE_IMPORTANT_MESSAGE, (float)m_id);
			(*it)->SendIM((im+message), m_id);

			continue;
		}
		*/

		(*it)->SendM(message);
		
	}
}

void Host::SendToClient(int clientID, const std::string& message, bool sendIM /*= false*/)
{
	int pos = SearchForClient(clientID);

	if(pos == -1)
		return;

	ClientData* cd = this->zClients[pos];
	cd->SendM(message);
	return;
	/*

	ClientData* cd = this->zClients[pos];
	if(!sendIM)
	{
		cd->SendM(message);
		return;
	}

	unsigned long m_id = cd->GetNextIPID();
	std::string im = this->zMessageConverter.Convert(MESSAGE_TYPE_IMPORTANT_MESSAGE, (float)m_id);
	cd->SendIM((im + message), m_id);
	*/
}

void Host::SendToClient(ClientData* cd, const std::string& message, bool sendIM /*= false*/)
{
	if(cd == NULL || message.size() == 0 )
		return;

	cd->SendM(message);
	return;
	/*
	if(!sendIM)
	{
		cd->SendM(message);
		return;
	}

	unsigned long m_id = cd->GetNextIPID();
	std::string im = this->zMessageConverter.Convert(MESSAGE_TYPE_IMPORTANT_MESSAGE, (float)m_id);
	cd->SendIM((message), m_id);
	*/
}

void Host::SendStartMessage()
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_START_GAME);

	this->SendToAllClients(msg, true);
}

void Host::SendPlayerActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> playerData;
	std::string mess = "";

	//Fetch player data
	std::vector<PlayerActor*> pl = this->zGameMode->GetActorHandlerPtr()->GetPlayers();
	for (auto it_Player = pl.begin(); it_Player < pl.end(); it_Player++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_PLAYER, (float)(*it_Player)->GetID());
		mess += (*it_Player)->ToMessageString(&this->zMessageConverter);

		playerData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = playerData.begin(); it_Message < playerData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

void Host::SendAnimalActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> animalData;
	std::string mess = "";

	//Fetch player data
	std::vector<AnimalActor*> al = this->zGameMode->GetActorHandlerPtr()->GetAnimals();
	for (auto it_Animal = al.begin(); it_Animal < al.end(); it_Animal++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_ANIMAL, (float)(*it_Animal)->GetID());
		mess += (*it_Animal)->ToMessageString(&this->zMessageConverter);

		animalData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = animalData.begin(); it_Message < animalData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

void Host::SendDynamicActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> dynamicData;
	std::string mess = "";

	//Fetch Static Objects data
	std::vector<DynamicProjectileObject*> dyp = this->zGameMode->GetActorHandlerPtr()->GetDynamicProjectiles();
	for (auto it_Dynamic = dyp.begin(); it_Dynamic < dyp.end(); it_Dynamic++)
	{
		Vector3 pos = (*it_Dynamic)->GetPosition();
		Vector4 rot = (*it_Dynamic)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_DYNAMIC_OBJECT, (float)(*it_Dynamic)->GetID());
		mess += (*it_Dynamic)->ToMessageString(&this->zMessageConverter);

		dynamicData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = dynamicData.begin(); it_Message < dynamicData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

bool Host::HasClients() const
{
	return !this->zClients.empty();
}

void Host::ReadMessages()
{
	static unsigned int MAX_MESSAGES_TO_READ = 10;
	unsigned int nrOfMessages = this->GetEventQueueSize();

	//No new messsages
	if(nrOfMessages == 0)
		return;

	nrOfMessages = min(nrOfMessages, MAX_MESSAGES_TO_READ);
	MaloW::ProcessEvent* pe;

	for (unsigned int i = 0; i < nrOfMessages; i++)
	{
		pe = PeekEvent();

		if ( MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(pe) )
		{
			HandleReceivedMessage(np->getID(), np->getMessage());
		}
		else if ( ClientConnectedEvent* CCE = dynamic_cast<ClientConnectedEvent*>(pe) )
		{
			// A Client Connected
			HandleNewConnection(CCE->GetClientChannel());
		}
		else if ( ClientDisconnectedEvent* CDE = dynamic_cast<ClientDisconnectedEvent*>(pe) )
		{
			HandleDisconnect( CDE->GetClientChannel() );
		}

		// Unhandled Message
		SAFE_DELETE(pe);
	}
}

void Host::SendErrorMessage(const long id, const std::string error_Message)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ERROR_MESSAGE, error_Message);
	this->SendToClient(id, msg);
}

std::string Host::CreateDeadPlayerObject(PlayerActor* pActor, DeadPlayerObjectActor** dpoActor)
{
	(*dpoActor) = new DeadPlayerObjectActor(true);

	std::vector<Item*> items;
	Inventory* inv = pActor->GetInventory();
	Equipment* eq = pActor->GetEquipment();

	std::string path = "Media/Models/Scale.obj";	

	Vector3 up = Vector3(0, 1, 0);//pActor->GetUpVector();
	Vector3 forward = Vector3(0, 0, 1);//pActor->GetDirection();
	forward.y = 0;
	Vector3 around = up.GetCrossProduct(forward);
	around.Normalize();
	float angle = 3.14f * 0.5f;

	PhysicsObject* pObj = pActor->GetPhysicObject();

	pObj->SetQuaternion(Vector4(0, 0, 0, 1));
	pObj->RotateAxis(around, angle);

	(*dpoActor)->SetActorModel(path);
	(*dpoActor)->SetScale(pActor->GetScale());
	
	(*dpoActor)->SetRotation(pActor->GetRotation());
	(*dpoActor)->SetPosition(pActor->GetPosition());
	
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_DEAD_PLAYER_OBJECT, (float)(*dpoActor)->GetID());
	msg += (*dpoActor)->ToMessageString(&this->zMessageConverter);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*dpoActor)->GetActorModel());

	std::vector<Item*> temp_items = inv->GetItems();
	RangedWeapon* rWeapon = eq->GetRangedWeapon();

	if (rWeapon)
	{
		items.push_back(rWeapon);
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)rWeapon->GetID());

		msg += rWeapon->ToMessageString(&this->zMessageConverter);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
	}
	MeleeWeapon* mWeapon = eq->GetMeleeWeapon();

	if (mWeapon)
	{
		items.push_back(mWeapon);
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)mWeapon->GetID());

		msg += mWeapon->ToMessageString(&this->zMessageConverter);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
	}
	Projectile* projectile = eq->GetProjectile();

	if (projectile)
	{
		items.push_back(projectile);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)projectile->GetID());

		msg += projectile->ToMessageString(&this->zMessageConverter);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
	}
	for (auto it = temp_items.begin(); it < temp_items.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN || (*it)->GetItemType() == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
		{
			Container* temp = dynamic_cast<Container*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Container when Creating Dead Player items");
				continue;
			}

			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());
			msg += temp->ToMessageString(&this->zMessageConverter);
			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_MATERIAL_SMALL_STICK || (*it)->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK ||
			(*it)->GetItemType() == ITEM_TYPE_MATERIAL_LARGE_STICK || (*it)->GetItemType() == ITEM_TYPE_MATERIAL_THREAD)
		{
			Material* temp = dynamic_cast<Material*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Material when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT || (*it)->GetItemType() == ITEM_TYPE_FOOD_WOLF_MEAT)
		{
			Food* temp = dynamic_cast<Food*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Food when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
		{
			Projectile* temp = dynamic_cast<Projectile*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Projectile when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW || (*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
		{
			RangedWeapon* temp = dynamic_cast<RangedWeapon*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Ranged Weapon when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE || (*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
		{
			MeleeWeapon* temp = dynamic_cast<MeleeWeapon*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Melee Weapon when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
	}

	(*dpoActor)->SetItems(items);

	return msg;
}

std::string Host::CreateDeadAnimalObject( AnimalActor* aAnimal, DeadAnimalObjectActor** daoActor )
{
	(*daoActor) = new DeadAnimalObjectActor(true);

	std::string path = aAnimal->GetActorModel();	

	//Rotate the object
	Vector3 up = Vector3(0, 1, 0);
	Vector3 forward = Vector3(0, 0, 1);
	forward.y = 0;
	Vector3 around = up.GetCrossProduct(forward);
	around.Normalize();
	float angle = 3.14f * 0.5f;

	PhysicsObject* aObj = aAnimal->GetPhysicObject();

	aObj->SetQuaternion(Vector4(0, 0, 0, 1));
	aObj->RotateAxis(around, angle);

	//Set information
	(*daoActor)->SetPosition(aAnimal->GetPosition());
	(*daoActor)->SetRotation(aAnimal->GetRotation());
	(*daoActor)->SetScale(aAnimal->GetScale());
	(*daoActor)->SetActorModel(path);

	//Creates a default item.
	Item* food = this->zGameMode->GetObjectCreatorPtr()->CreateItemFromDefault(OBJECT_TYPE_FOOD_DEER_MEAT);

	//Generates food drops for the dead animal.
	if (!(*daoActor)->GenerateDrop(food))
	{
		MaloW::Debug("GenerateDrop returned false in CreateDeadAnimalObject.");
		SAFE_DELETE(food);

		return "";
	}

	//Creates a message to notify clients about the dead animal.
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_DEAD_ANIMAL_OBJECT, (float)(*daoActor)->GetID());
	msg += (*daoActor)->ToMessageString(&this->zMessageConverter);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*daoActor)->GetActorModel());

	//Gather food information from the copy.
	std::string temp_msg = food->ToMessageString(&this->zMessageConverter);
	std::string temp_msg_finished = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);

	unsigned int size = (*daoActor)->GetItems().size();

	for (unsigned int i = 0; i < size-1; i++)
	{
		msg += temp_msg;
		msg += temp_msg_finished;
	}

	SAFE_DELETE(food);

	return msg;
}

void Host::SendNewObjectMessage(StaticObjectActor* staticObj)
{
	std::string msg;
	Vector3 pos = staticObj->GetPosition();
	Vector3 scale = staticObj->GetScale();
	Vector4 rot = staticObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)staticObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, staticObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)staticObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)staticObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)staticObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, staticObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, staticObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, staticObj->GetIconPath());

	this->SendToAllClients(msg, true);
}

void Host::SendNewObjectMessage(DynamicObjectActor* dynamicObj)
{
	std::string msg;
	Vector3 pos = dynamicObj->GetPosition();
	Vector3 scale = dynamicObj->GetScale();
	Vector4 rot = dynamicObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_DYNAMIC_OBJECT, (float)dynamicObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, dynamicObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)dynamicObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)dynamicObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)dynamicObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, dynamicObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, dynamicObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, dynamicObj->GetIconPath());
	//ProjectileObject* projectileObj = dynamic_cast<ProjectileObject*>(dynamicObj);
	//if (projectileObj)
	//{
		//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, projectileObj->GetDamage());
	//}
	
	this->SendToAllClients(msg, true);
}

void Host::SendNewObjectMessage(AnimalActor* animalObj)
{
	std::string msg;
	Vector3 pos = animalObj->GetPosition();
	Vector3 scale = animalObj->GetScale();
	Vector4 rot = animalObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_ANIMAL, (float)animalObj->GetID());
	msg += animalObj->ToMessageString(&this->zMessageConverter);
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, animalObj->GetActorModel());

	this->SendToAllClients(msg, true);
}

void Host::HandleReceivedMessage( const unsigned int &ID, const std::string &message )
{
	std::vector<std::string> msgArray;
	msgArray = this->zMessageConverter.SplitMessage(message); 
	
	// Empty Array
	if ( msgArray.size() == 0 ) return;

	int c_index = SearchForClient(ID);
	PlayerActor* p_actor = dynamic_cast<PlayerActor*>(this->zGameMode->GetActorHandlerPtr()->GetActor(ID, ACTOR_TYPE_PLAYER));

	//Handles updates from client.
	if(msgArray[0].find(M_CLIENT_DATA.c_str()) == 0 && (p_actor))
	{
		this->HandlePlayerUpdate(p_actor, this->zClients[c_index], msgArray);
	}
	//Handles key presses from client.
	else if(msgArray[0].find(M_KEY_DOWN.c_str()) == 0 && (p_actor))
	{
		int key = this->zMessageConverter.ConvertStringToInt(M_KEY_DOWN, msgArray[0]);

		this->zGameMode->HandleKeyPress(ID, key);
		//this->HandleKeyPress(p_actor, msgArray[0]);
	}
	//Handles key releases from client.
	else if(msgArray[0].find(M_KEY_UP.c_str()) == 0 && (p_actor))
	{
		int key = this->zMessageConverter.ConvertStringToInt(M_KEY_UP, msgArray[0]);

		this->zGameMode->HandleKeyRelease(ID, key);
		//this->HandleKeyRelease(p_actor, msgArray[0]);
	}
	//Handles Pings from client.
	else if(msgArray[0].find(M_PING.c_str()) == 0 && (c_index != -1))
	{
		this->zClients[c_index]->HandlePingMsg();
	}
	//Handles ready from client.
	else if(msgArray[0].find(M_READY_PLAYER.c_str()) == 0 && (c_index != -1))
	{
		this->zClients[c_index]->SetReady(true);
	}
	//Handles Ack Messages
	else if(msgArray[0].find(M_ACKNOWLEDGE_MESSAGE.c_str()) == 0 && (c_index != -1))
	{
		int m_id = this->zMessageConverter.ConvertStringToInt(M_ACKNOWLEDGE_MESSAGE, msgArray[0]);
		this->zClients[c_index]->RemoveIM(m_id);
	}
	//Handle Item usage in Inventory
	else if(msgArray[0].find(M_ITEM_USE.c_str()) == 0 && (c_index != -1))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
		this->HandleItemUse(p_actor, objID);
	}
	//Handle UnEquip Item in Equipment
	else if(msgArray[0].find(M_UNEQUIP_ITEM.c_str()) == 0 && (c_index != -1))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
		int eq_Slot = -1;
		if (msgArray.size() > 1)
		{
			eq_Slot = this->zMessageConverter.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
		}
		this->HandleUnEquipItem(p_actor, objID, eq_Slot);
	}
	//Handles Equipped Weapon usage
	else if(msgArray[0].find(M_WEAPON_USE.c_str()) == 0 && (c_index != -1))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);
		this->HandleWeaponUse(p_actor, objID);
	}
	//Handles Pickup Object Requests from Client
	else if(msgArray[0].find(M_PICKUP_ITEM.c_str()) == 0 && (p_actor))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_PICKUP_ITEM, msgArray[0]);
		this->HandlePickupItem(p_actor, objID);
	}
	else if(msgArray[0].find(M_LOOT_ITEM.c_str()) == 0 && (p_actor))
	{
		long objID = this->zMessageConverter.ConvertStringToInt(M_LOOT_ITEM, msgArray[0]);
		int type = -1;
		long itemID = -1;
		if (msgArray.size() > 2)
		{
			itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_ID, msgArray[1]);
			type = this->zMessageConverter.ConvertStringToInt(M_ITEM_TYPE, msgArray[2]);

			if (this->HandleLootItem(p_actor, objID, itemID, type))
			{
				this->SendRemoveDeadPlayerItem(p_actor->GetID(), objID, itemID, type);
			}

		}
		else
		{
			MaloW::Debug("Msg array size is to short size: " + MaloW::convertNrToString((float)msgArray.size()) + " Expected size 3");
		}
	}
	//Handles Drop Item Requests from Client
	else if(msgArray[0].find(M_DROP_ITEM.c_str()) == 0 && (p_actor))
	{
		int objID = this->zMessageConverter.ConvertStringToInt(M_DROP_ITEM, msgArray[0]);
		this->HandleDropItem(p_actor, objID);
	}
	//Handles user data from client. Used when the player is new.
	else if(msgArray[0].find(M_USER_DATA.c_str()) == 0 && (c_index != -1))
	{
		this->CreateNewPlayer(this->zClients[c_index], msgArray);
	}
	//Handles if client disconnects.
	else if(msgArray[0].find(M_CONNECTION_CLOSED.c_str()) == 0)
	{
		this->KickClient(ID);
	}
	//Handles if not of the above.
	else
	{
		MaloW::Debug("Warning: The host cannot handle the message \"" + message + "\" in HandleReceivedMessages.");
	}
}

void Host::HandlePlayerUpdate(PlayerActor* pl, ClientData* cd, const std::vector<std::string> &data)
{
	
	for(auto it = data.begin() + 1; it < data.end(); it++)
	{
		char key[512];
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));
		
		if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			Vector3 dir = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it));
			pl->SetDirection(dir);
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			Vector3 up = this->zMessageConverter.ConvertStringToVector(M_UP, (*it));
			pl->SetUpVector(up);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rot = this->zMessageConverter.ConvertStringToQuaternion(M_ROTATION, (*it));
			pl->SetRotation(rot);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			float frameTime = this->zMessageConverter.ConvertStringToFloat(M_FRAME_TIME, (*it));
			pl->SetFrameTime(frameTime);
		}
		else
			MaloW::Debug("Unknown message in HandlePlayerUpdate.");
	}

	//Update Latency
	float latency;

	if(!cd->CalculateLatency(latency))
		return;

	pl->SetLatency(latency);
}

void Host::HandleClientNackIM()
{

	if(!HasClients())
		return;
	std::vector<int> clientsToKick;

	for(auto it = this->zClients.begin(); it < this->zClients.end(); it++)
	{
		(*it)->HandleNackIM(this->zDeltaTime);

		if((*it)->GetNrOfExceededIM() > 0)
			clientsToKick.push_back((*it)->GetClient()->GetClientID());
	}

	for (auto it = clientsToKick.begin(); it < clientsToKick.end(); it++)
	{
		KickClient((*it), true, "No Ack on message.");
	}

}

int Host::SearchForClient(const int ID) const
{
	if(!HasClients())
		return -1;

	for (unsigned int i = 0; i < this->zClients.size(); i++)
	{
		if(this->zClients.at(i)->GetClient()->GetClientID() == ID)
		{
			return i;
		}
	}

	return -1;
}

void Host::BroadCastServerShutdown()
{
	std::string mess = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_SHUTDOWN);
	SendToAllClients(mess);
}

void Host::PingClients()
{
	if(!HasClients())
		return;

	ClientData* cd; 
	MaloW::ClientChannel* ch;

	for(unsigned int i = 0; i < (unsigned int)zClients.size(); i++)
	{
		cd = zClients.at(i);
		ch = cd->GetClient();

		//If client has not been pinged.
		if(!cd->HasBeenPinged())
		{
			//If it was x sec ago we sent a ping, don't send a ping.
			if(cd->GetCurrentPingTime() < zPingMessageInterval)
				cd->IncPingTime(zDeltaTime);

			//else send ping.
			else
			{
				cd->SetCurrentPingTime(0.0f);
				ch->Send(this->zMessageConverter.Convert(MESSAGE_TYPE_PING));
				cd->SetPinged(true);
			}
		}
		//If he have sent a ping.
		else
		{
			//If we sent a ping x sec ago, drop the client.
			if(cd->GetCurrentPingTime() > zTimeOut)
			{
				KickClient(ch->GetClientID());
			}
			else
				cd->IncPingTime(zDeltaTime);

		}
	}
}

float Host::Update()
{
	//Update Timer
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->zStartime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zStartime = currentTime;

	//Update the objects in the world.
	this->zGameMode->Update(zDeltaTime);

	return this->zDeltaTime;
}

void Host::UpdateObjects()
{
	

	std::vector<CollisionEvent> ce;
	//CheckCollisions is not complete.
	this->zGameMode->GetActorHandlerPtr()->CheckCollisions();
	ce = this->zGameMode->GetActorHandlerPtr()->CheckProjectileCollisions();

	//Iterate projectiles and converts them to static if they have stopped
	std::vector<DynamicProjectileObject*> dynamicProjectileObj = this->zGameMode->GetActorHandlerPtr()->GetDynamicProjectiles();

	for (auto it_proj = dynamicProjectileObj.begin(); it_proj < dynamicProjectileObj.end(); it_proj++)
	{
		if (!(*it_proj)->IsMoving())
		{
			HandleConversion((*it_proj));

			if(!this->zGameMode->GetActorHandlerPtr()->RemoveDynamicProjectileActor((*it_proj)->GetID()))
				MaloW::Debug("Failed to Remove Object in Host::UpdateObjects");
		}
	}

	//Iterate players and see if they are dead.
	std::vector<PlayerActor*> pActors = this->zGameMode->GetActorHandlerPtr()->GetPlayers();
	for (auto it_player = pActors.begin(); it_player < pActors.end(); it_player++)
	{
		if (!(*it_player)->IsAlive())
		{
			OnBioActorDeath(*it_player);
		}
		else
		{
			//position = (*it_player)->GetPosition();
			//if (this->zWorld->IsBlockingAt(position.GetXZ()))
			//{
			//	(*it_player)->RewindPosition();
			//}
		}
	}

	/*
	//Iterate animals and see if they are dead.
	std::vector<AnimalActor*> pActors = this->zActorHandler->GetAnimals();
	for (auto it_animals = pActors.begin(); it_animals < pActors.end(); it_animals++)
	{
		if (!(*it_animals)->IsAlive())
		{
			OnPlayerDeath((*it_animals)->GetID());
		}
		else
		{
			//position = (*it_player)->GetPosition();
			//if (this->zWorld->IsBlockingAt(position.GetXZ()))
			//{
			//	(*it_player)->RewindPosition();
			//}
		}
	}
	*/
}

bool Host::KickClient(const int ID, bool sendAMessage, std::string reason)
{
	int index = SearchForClient(ID);

	ClientData* temp_c = zClients.at(index);

	std::string mess;
	bool removed = false;

	if(index == -1)
	{
		MaloW::Debug("Can't find client to kick in KickClient in Host.cpp.");
		return false;
	}

	if(sendAMessage)
	{
		mess = this->zMessageConverter.Convert(MESSAGE_TYPE_KICKED, reason);
		temp_c->SendM(mess);
	}
	//Create Dead Player Object
	BioActor* bActor = dynamic_cast<BioActor*>(this->zGameMode->GetActorHandlerPtr()->GetActor(ID, ACTOR_TYPE_PLAYER));
	OnBioActorRemove(bActor);

	//Create a remove player message.
	mess = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_PLAYER, (float)ID);

	//Remove the player

	this->zClients.erase(zClients.begin() + index);
	this->zGameMode->GetActorHandlerPtr()->RemovePlayerActor(ID);

	SAFE_DELETE(temp_c);

	removed = true;
	MaloW::Debug("Client"+MaloW::convertNrToString((float)ID)+" removed from server.");
	
	
	//Notify clients
	this->SendToAllClients(mess, true);

	return removed;
}

void Host::OnBioActorRemove( BioActor* actor )
{
	if (!actor)
		return;

	int type = actor->GetActorType();
	
	PlayerActor* pActor = NULL;
	AnimalActor* aActor = NULL;
	DeadPlayerObjectActor* dpoActor = NULL;
	DeadAnimalObjectActor* daoActor = NULL;
	std::string msg = "";

	if(type == ACTOR_TYPE_PLAYER)
	{
		pActor = dynamic_cast<PlayerActor*>(actor);
		msg = this->CreateDeadPlayerObject(pActor, &dpoActor);
	}
	else if(type == ACTOR_TYPE_ANIMAL)
	{
		aActor = dynamic_cast<AnimalActor*>(actor);
		msg = this->CreateDeadAnimalObject(aActor, &daoActor);
	}
	else
		return;

	this->zGameMode->GetActorHandlerPtr()->AddNewDeadPlayer(dpoActor);

	this->SendToAllClients(msg, true);
}

void Host::OnBioActorDeath( BioActor* actor )
{
	if(!actor)
		return;

	OnBioActorRemove(actor);

	int type = actor->GetActorType();
	int messageType = 0;
	std::string msg = "";
	
	if (type == ACTOR_TYPE_PLAYER)
	{
		PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor);
		messageType = MESSAGE_TYPE_DEAD_PLAYER;

		RespawnPlayer(pActor);
		
		//

		this->zGameMode->GetActorHandlerPtr()->RemovePlayerActor(pActor->GetID());
	}
	else if(type == ACTOR_TYPE_ANIMAL)
	{
		messageType = MESSAGE_TYPE_DEAD_ANIMAL;
		this->zGameMode->GetActorHandlerPtr()->RemoveAnimalActor(actor->GetID());
	}
	else
	{
		MaloW::Debug("Cant find Actor in Host::OnPlayerDeath");
		return;
	}

	msg = this->zMessageConverter.Convert(messageType, (float)actor->GetID());
	SendToAllClients(msg, true);
	
}

bool Host::IsAlive() const
{
	return this->stayAlive;
}

void Host::CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data )
{
	std::string mess;
	
	PlayerActor* pi = new PlayerActor(cd->GetClient()->GetClientID());

	for (auto it_m = data.begin() + 1; it_m < data.end(); it_m++)
	{
		char key[512];
		sscanf_s((*it_m).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			std::string uModel = this->zMessageConverter.ConvertStringToSubstring(M_MESH_MODEL, (*it_m));
			if(uModel != "")
				pi->SetActorModel(uModel);
		}
		else if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			Vector3 uDir = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it_m));
			pi->SetDirection(uDir);
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			Vector3 uUp = this->zMessageConverter.ConvertStringToVector(M_UP, (*it_m));
			pi->SetUpVector(uUp);
		}
	}

	//Debug Pos
	int currentPoint = this->zGameMode->GetActorHandlerPtr()->GetPlayers().size() % this->zMaxClients;
	Vector3 position = CalculateSpawnPoint(currentPoint, this->zMaxClients, 20);
	PhysicsObject* pObj = this->zGameMode->GetActorHandlerPtr()->GetPhysicEnginePtr()->CreatePhysicsObject(pi->GetActorModel(), position);
	pi->SetPhysicObject(pObj);
	pi->SetScale(Vector3(0.05f, 0.05f, 0.05f));

	if(!pObj)
		MaloW::Debug("Error in function AddNewPlayer in ActorHandler: PhysicObj is null.");

	/*End debug*/	//Add new player to the list
	this->zGameMode->GetActorHandlerPtr()->AddNewPlayer(pi);

	pi->AddObserver(this);
	zAnchorPlayerMap[pi] = this->zWorld->CreateAnchor();
	zAnchorPlayerMap[pi]->radius = 200.0f;

	//Gather New player information
	mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (float)pi->GetID());
	mess += pi->ToMessageString(&this->zMessageConverter);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, pi->GetActorModel());

	//Send new player to players
	SendToAllClients(mess, true);
	if (this->zGameStarted)
	{
		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_START_GAME);
		this->SendToClient(pi->GetID(), msg, true);
	}

}

void Host::RespawnPlayer(PlayerActor* pActor)
{
	std::string model = pActor->GetActorModel();
	PlayerActor* new_Player = new PlayerActor(pActor->GetID());

	int currentPoint = this->zGameMode->GetActorHandlerPtr()->GetPlayers().size() % this->zMaxClients;
	Vector3 position = CalculateSpawnPoint(currentPoint, this->zMaxClients, 20);
	PhysicsObject* pObj = this->zGameMode->GetActorHandlerPtr()->GetPhysicEnginePtr()->CreatePhysicsObject(model, position);
	new_Player->SetPhysicObject(pObj);
	new_Player->SetActorModel(model);
	new_Player->SetScale(Vector3(0.05f, 0.05f, 0.05f));

	if(!pObj)
		MaloW::Debug("Error in function AddNewPlayer in ActorHandler: PhysicObj is null.");

	/*End debug*/	//Add new player to the list
	this->zGameMode->GetActorHandlerPtr()->AddNewPlayer(new_Player);

	new_Player->AddObserver(this);
	zAnchorPlayerMap[new_Player] = this->zWorld->CreateAnchor();
	zAnchorPlayerMap[new_Player]->radius = 200.0f;

	//Gather New player information
	Vector3 pos = new_Player->GetPosition();
	Vector3 scale = new_Player->GetScale();
	Vector4 rot = new_Player->GetRotation();

	std::string mess;

	mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (float)new_Player->GetID());
	mess += new_Player->ToMessageString(&this->zMessageConverter);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, new_Player->GetActorModel());


	//Send new player to players
	SendToAllClients(mess, true);
	if (this->zGameStarted)
	{
		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_START_GAME);
		this->SendToClient(new_Player->GetID(), msg, true);
	}
}

void Host::CreateNewGhostActor(PlayerActor* pActor)
{
	GhostActor* gActor = new GhostActor(pActor->GetID(), pActor->GetPosition());
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_GHOST_ACTOR, pActor->GetID());

	msg += gActor->ToMessageString(&this->zMessageConverter);

	this->SendToAllClients(msg, true);
}

void Host::GetExistingObjects(std::vector<std::string>& static_Objects)
{
	std::string mess;
	std::vector<FoodObject*> static_Food = this->zGameMode->GetActorHandlerPtr()->GetFoods();
	//Gets Static Food Objects Data
	for(auto it = static_Food.begin(); it < static_Food.end(); it++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += (*it)->ToMessageString(&this->zMessageConverter);

		static_Objects.push_back(mess);

	}
	std::vector<WeaponObject*> static_Weapon = this->zGameMode->GetActorHandlerPtr()->GetWeapons();
	//Gets Static Weapon Objects Data
	for(auto it = static_Weapon.begin(); it < static_Weapon.end(); it++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += (*it)->ToMessageString(&this->zMessageConverter);

		static_Objects.push_back(mess);
	}

	std::vector<ContainerObject*> static_Container = this->zGameMode->GetActorHandlerPtr()->GetContainers();
	//Gets Static Container Objects Data
	for(auto it = static_Container.begin(); it < static_Container.end(); it++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += (*it)->ToMessageString(&this->zMessageConverter);

		static_Objects.push_back(mess);
	}

	std::vector<StaticProjectileObject*> static_Projectile = this->zGameMode->GetActorHandlerPtr()->GetStaticProjectiles();
	//Gets Static Projectile Objects Data
	for(auto it = static_Projectile.begin(); it < static_Projectile.end(); it++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += (*it)->ToMessageString(&this->zMessageConverter);

		static_Objects.push_back(mess);
	}

	std::vector<MaterialObject*> static_Material = this->zGameMode->GetActorHandlerPtr()->GetMaterials();
	//Gets Static Weapon Objects Data
	for(auto it = static_Material.begin(); it < static_Material.end(); it++)
	{
		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += (*it)->ToMessageString(&this->zMessageConverter);

		static_Objects.push_back(mess);
	}
}

int Host::GetNrOfPlayers() const
{
	return this->zClients.size();
}

void Host::OnEvent(Event* e)
{
	if ( WorldDeletedEvent* WDE = dynamic_cast<WorldDeletedEvent*>(e) )
	{
		if ( zWorld ) zWorld = 0;
	}
	else if ( EntityRemovedEvent *ERE = dynamic_cast<EntityRemovedEvent*>(e) )
	{
		//zTargetedEntities.erase(ERE->entity);
	}
	else if( PlayerUpdatedEvent* PUE = dynamic_cast<PlayerUpdatedEvent*>(e) )
	{
		Vector3 playerTempPos = PUE->playerActor->GetPosition();
		
		if(playerTempPos.x >= 0 && playerTempPos.z >= 0 && playerTempPos.x < zWorld->GetWorldSize().x && playerTempPos.z < zWorld->GetWorldSize().y )
		{
			Vector3 oldPos = PUE->prevPos; 
			float yPos = this->zWorld->CalcHeightAtWorldPos(Vector2(playerTempPos.x, playerTempPos.z));
			Vector3 dir = playerTempPos - oldPos;
			Vector3 groundNormal = this->zWorld->CalcNormalAt(Vector2(playerTempPos.x, playerTempPos.z));

			playerTempPos.y -= (9.82f * this->zDeltaTime);			
			if(playerTempPos.y < yPos)
				playerTempPos.y = yPos;

			//dir.y = groundNormal.y;
			//dir.y = yPos;
			dir.Normalize();
			Vector3 tempGround = groundNormal;
			tempGround.y = 0;
			tempGround.Normalize();
			float dot = dir.GetDotProduct(tempGround);
			if( groundNormal.y <= 0.5f )
			{
				Vector3 newPlayerTempPos = playerTempPos + (tempGround * (zDeltaTime * 4));

				float yPosNew = this->zWorld->CalcHeightAtWorldPos(Vector2(newPlayerTempPos.x, newPlayerTempPos.z));
				newPlayerTempPos.y += -9.82f * zDeltaTime;
				if(newPlayerTempPos.y < yPosNew)
					newPlayerTempPos.y = yPosNew;
				PUE->validMove = false;
				PUE->prevPos = newPlayerTempPos;
			}
			else if(dot > 0.2)
			{
				PUE->validMove = true;
//				playerTempPos.y += -1.82 * zDeltaTime;
				playerTempPos.y += -9.82f * zDeltaTime;
				if(playerTempPos.y < yPos)
					playerTempPos.y = yPos;

				PUE->playerActor->SetPosition(playerTempPos);
				this->zAnchorPlayerMap[PUE->playerActor]->position = Vector2(playerTempPos.x, playerTempPos.z);
			}			
			else if(groundNormal.y > 0.7f)
			{
				playerTempPos.y = yPos;
				PUE->playerActor->SetPosition(playerTempPos);
				this->zAnchorPlayerMap[PUE->playerActor]->position = Vector2(playerTempPos.x, playerTempPos.z);
				PUE->validMove = true;
			}
			

		}
		else
		{
			PUE->validMove = true;
		}
	}
}

void Host::HandleDisconnect( MaloW::ClientChannel* channel )
{
	KickClient(channel->GetClientID());
}
